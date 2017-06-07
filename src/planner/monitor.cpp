
#include "ros/ros.h"
#include <ses/RobotStatus.h>
#include <ses/step.h>
#include <sstream>
#include <string.h>
#include <signal.h>
#include <ses/Path.h>
#include "grid.h"
#include "myTuple.h"
#include "pathCell.h"
#include "allocation.h"

using namespace std;

#define MAX_ROBOTS_NUM 20

enum robotState {idle,traveling,covering,done,dead};

unsigned int teamSize;
unsigned int robotsCount = 0;
vector<myTuple> locations;
vector<int> team;
vector<bool> lives;
allocation* al;
int sekelton = -1;
bool started = false;//The coverage started
bool end = false;//Coverage should be stopped or completed successfully

//team_status topic
ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;

//Subscriber to steps topic
ros::Subscriber steps_sub;

//Publisher to paths topic
ros::Publisher path_pub;


int bury(int id);
string path2str(vector<pathCell*> path);
string probs2str(vector<pathCell*> path);
void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg);
void stepCallback(const ses::step::ConstPtr& step_msg);
void publishPath(int robot_id,robotState state, vector<pathCell*> path);
void handler(int signum);


int main(int argc, char **argv)
{
	//nh.getParam("group_size",teamSize);

	char *teamSizeStr = argv[1];
	teamSize = atoi(teamSizeStr);

	lives.resize(teamSize);
	for (int i = 0; i < teamSize; ++i)
	{
		lives[i] = false;
	}

	locations.resize(teamSize);
	team.resize(teamSize);

	// Check that robot id is between 0 and MAX_ROBOTS_NUM
	if (teamSize > MAX_ROBOTS_NUM || teamSize < 1 ) {
	    ROS_ERROR("The team size must be an integer number between 1 and %d", MAX_ROBOTS_NUM);
	    return -1;
	}

	ros::init(argc, argv, "monitor");
	ros::NodeHandle nh;

	team_status_pub = nh.advertise<ses::RobotStatus>("team_status", 10);
	team_status_sub = nh.subscribe("team_status", 1, &teamStatusCallback);

	path_pub = nh.advertise<ses::Path>("paths", 10);
	steps_sub = nh.subscribe("steps", 1, &stepCallback);

	ROS_INFO("Waiting for robots to connect...");

	while(!end){
		 ros::spinOnce();
	}
	cout<<"finished"<<endl;
}

//alarm handler: Each alert checks the status of the robots on the team if they are still alive
void handler(int signum) {
	cout<<"monitor::alarm"<<endl;
	cout<<"monitor:: live robots"<<robotsCount<<endl;
    for (int i = 0; i < lives.size(); ++i)
    {
    	if(!lives[i]){
    		if(team[i]!= sekelton){
    			robotsCount--;
    			bury(i);
    		}
    	}
    	lives[i] = false;
    }
    if(robotsCount > teamSize || robotsCount <= 0){
    	cout<<"monitor::whole team is dead :("<<endl;
    	end = true;
    }else{
    	alarm(12);
    	signal(SIGALRM, handler);
    }
}

int bury(int id){
	team[id] = sekelton;
	myTuple place = locations[id];
	grid* g = grid::getInstance();
	cout<<"monitor:robot "<<id<<" died at "<<place.returnFirst()<<","<<place.returnSecond()<<endl;
	pathCell* c = g->getCellAt(place.returnFirst(),place.returnSecond());
	c->setProb(1.0); //No one can pass another cell where a robot has died
	al->unAssign(id);//Update the provisioning department by leaving the robot
}


void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg)
{
	
	int robot_id = status_msg->robot_id;
	if (!lives[robot_id]) {
		lives[robot_id] = true;
		if(!started){ //Before the cover starts, make sure that all the robots have arrived
			ROS_INFO("Robot %d is ready!\n", robot_id);
			int x = status_msg->start_x;
			int y = status_msg->start_y;
			myTuple m(x,y);
			locations[robot_id] = m;
			team[robot_id] = robot_id;
			robotsCount++;
			if (robotsCount == teamSize) {
				ROS_INFO("All robots GO!");
				
				//ctor<myTuple> locations(teamSize,myTuple(-1,-1));
				vector<int> team(teamSize,-1);
				for (int i = 0; i < teamSize; ++i)
				{
					//locations[i] = locs[i];
					team[i] = i;
				}
				//Initialization of the allocation class for the robots that arrived
			 	al = new allocation(team,locations);
			 	started = true;

			 	//Operation of the alarm mechanism to check the condition of the robots
			 	alarm(12);
	    		signal(SIGALRM, handler);

	    		//publish a message to all robots that they can start walking
				ses::RobotStatus status_msg;
				status_msg.header.stamp = ros::Time::now();
				status_msg.header.frame_id = "monitor";
				team_status_pub.publish(status_msg);
			}
		}
	}
}


void publishPath(int robot_id,robotState state, vector<pathCell*>path){

	string probs = probs2str(path);
	string newPath = path2str(path);
	//debug code
	cout<< "monitor::path for "<<robot_id<< ": "<< newPath << endl;
	//debug code

	ses::Path path_msg;

	path_msg.robot_id = robot_id;
	path_msg.path = newPath;
	path_msg.state = state;
	path_msg.probs = probs;

	sleep(1.0);
	path_pub.publish(path_msg);
}


//Given the location and state of the robot, the function determines what path the robot must go to, and publishes it
void stepCallback(const ses::step::ConstPtr& step_msg){
	int robot_id = step_msg->robot_id;
	int x = step_msg->first_location;
	int y = step_msg->second_location;
	myTuple m(x,y);
	locations[robot_id] = m;
	cout<<"monitor: "<<robot_id<<"is now at "<<locations[robot_id].returnFirst()<<","<<locations[robot_id].returnSecond()<<endl;
	robotState state = (robotState)step_msg->state;
	grid* g = grid::getInstance();
	pathCell* c = g->getCellAt(step_msg->first_location, step_msg->second_location);

	//debug code
	string strstate;
	if(state == idle){strstate = "idle";}
	if(state == traveling){strstate = "traveling";}
	if(state == covering){strstate = "covering";}
	if(state == dead){strstate = "dead";}
	if(state == done){strstate = "done";}
	cout<< "monitor::state of the robot: "<< strstate<< endl;
	//

	c->changeState(); //Update the state of the cell reported by the robot 
	

	//publish only if the status changed
	if(state == idle || state == done){
		vector<pathCell*> path = al->allocateStartArea(robot_id);
		if(path.size()==1){
			path = al->areaCoverage(c->getLocation(),robot_id);
			publishPath(robot_id,covering,path);
			return;
		}
		publishPath(robot_id,traveling,path);
		return;		
	}	
	if(step_msg->is_the_last){//the robot finished his path
		if(state == traveling){//the robot found his area
			vector<pathCell*> path = al->areaCoverage(c->getLocation(),robot_id);
			if(path.size() == 0){//area of one cell
				path = al->allocateNextArea(c-> getLocation(), robot_id);
				publishPath(robot_id,traveling,path);
				return;
			}
			publishPath(robot_id,covering,path);
			return;
		}
		if(state == covering){//the robot finished to cover his area
			vector<pathCell*> path = al->allocateNextArea(c-> getLocation(), robot_id);
			if(path.size() == 0){
				publishPath(robot_id,done,path);
				return;
			}
			publishPath(robot_id,traveling,path);
			return;
		}
	}
}

string path2str(vector<pathCell*> path){
	string newS = "";
	for (int i = 0; i < path.size(); ++i)
	{	
		ostringstream ss1,ss2;
		ss1 << path[i]->getLocation().returnFirst();
		ss2 << path[i]->getLocation().returnSecond();
		newS +=  ss1.str() +  "," + ss2.str();
		if (i!= path.size() -1) {
			newS += " ";
		}

	}
	return newS;
}

string probs2str(vector<pathCell*> path){
	string newP = "";
	for (int i = 0; i < path.size(); ++i)
	{
		ostringstream ss;
		ss << path[i]->getProb();
		cout<< ss.str() <<endl;
		newP +=  ss.str(); 
		if (i!= path.size() -1) {
			newP += " ";
		}
	}
	return newP;

}

