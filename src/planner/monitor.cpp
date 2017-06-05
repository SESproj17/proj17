
#include "ros/ros.h"
#include <ses/RobotStatus.h>
#include <ses/step.h>
#include <sstream>
#include <string.h>
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
bool robotsReady[MAX_ROBOTS_NUM];
vector<myTuple> locs(MAX_ROBOTS_NUM,myTuple(-1,-1));
vector<int> ids;
allocation* al;
int sekelton = -1;


ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;

ros::Subscriber steps_sub;
ros::Publisher path_pub;

void bury(int robot_id);
string path2str(vector<pathCell*> path);
string probs2str(vector<pathCell*> path);
void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg);
void stepCallback(const ses::step::ConstPtr& step_msg);
//void publishPath(robotState state, string path, string area, int robot_id);
void publishPath(int robot_id,robotState state, string path,string probs);
void handler(int signum);


int main(int argc, char **argv)
{
	if (argc < 2) {
		ROS_ERROR("You must specify team size.");
		return -1;
	}

	char *teamSizeStr = argv[1];
	teamSize = atoi(teamSizeStr);

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

	ros::spin();
}


void handler(int signum) {
    publishAliveStatus();
    alarm(1);
    signal(SIGALRM, handler);
}


void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg)
{
	int robot_id = status_msg->robot_id;
	int x = status_msg->start_x;
	int y = status_msg->start_y;
	if (!robotsReady[robot_id]) {
		ROS_INFO("Robot %d is ready!\n", robot_id);
		robotsReady[robot_id] = true;
		myTuple m(x,y);
		locs[robot_id] = m;
		robotsCount++;
		
		
		if (robotsCount == teamSize) {

			ROS_INFO("All robots GO!");
			vector<myTuple> locations(teamSize,myTuple(-1,-1));
			vector<int> team(teamSize,-1);
			for (int i = 0; i < teamSize; ++i)
			{
				locations[i] = locs[i];

				team[i] = i;
			}
		 	al = new allocation(team,locations);

		 	alarm(1);
    		signal(SIGALRM, handler);

			ses::RobotStatus status_msg;
			status_msg.header.stamp = ros::Time::now();
			status_msg.header.frame_id = "monitor";
			team_status_pub.publish(status_msg);
		}
	}
}


void publishPath(int robot_id,robotState state, string path,string givenprobs){

	ses::Path path_msg;

	//path_msg.header.stamp = ros::Time::now();
	path_msg.robot_id = robot_id;
	path_msg.path = path;
	path_msg.state = state;
	path_msg.probs = givenprobs;

	sleep(1.0);
	path_pub.publish(path_msg);
}


void stepCallback(const ses::step::ConstPtr& step_msg){

	

	int robot_id = step_msg->robot_id;
	robotState state = (robotState)step_msg->state;
	grid* g = grid::getInstance();
	pathCell* c = g->getCellAt(step_msg->first_location, step_msg->second_location);

	cout<<"monitor::catched a step of "<<robot_id<<endl;
	

	//debug code
	string strstate;
	if(state == idle){strstate = "idle";}
	if(state == traveling){strstate = "traveling";}
	if(state == covering){strstate = "covering";}
	if(state == dead){strstate = "dead";}
	if(state == done){strstate = "done";}
	cout<< "monitor::state of the robot: "<< strstate<< endl;
	//

	c->changeState();
	if (state == dead) {
		c->setProb(1.0);
		al->bury(robot_id);
		bury(robot_id);
		
	}

	//publish only if the status changed!
	if(state == idle || state == done){//done-??????
		vector<pathCell*> path = al->allocateStartArea(robot_id);
		if(path.size()==1){
			path = al->areaCoverage(c->getLocation(),robot_id);
			string probs = probs2str(path);
			string newPath = path2str(path);
			//debug code
			cout<< "monitor::path for "<<robot_id<< ": "<< newPath << endl;
			//debug code
			publishPath(robot_id,covering,newPath,probs);
			return;

		}else{
			string probs = probs2str(path);
			string newPath = path2str(path);
			//debug code
			cout<< "monitor::path for "<<robot_id<< ": "<< newPath << endl;
			//debug code
			publishPath(robot_id,traveling,newPath,probs);
			return;
		}
				
	}
	//cout << "monitor: is_the_last " << (int)step_msg->is_the_last << endl;
	if(step_msg->is_the_last){
		if(state == traveling){//robi found his area
			string newPath;
			vector<pathCell*> path = al->areaCoverage(c->getLocation(),robot_id);
			if(path.size() == 0){
				path = al->allocateNextArea(c-> getLocation(), robot_id);
				newPath = path2str(path);
				string probs = probs2str(path);
				publishPath(robot_id,traveling,newPath,probs);
				//debug code
				cout<< "monitor::path for "<<robot_id<< ": "<< newPath << endl;
				//debug code
				return;
			}
			newPath = path2str(path);
			string probs = probs2str(path);
			//debug code
			cout<< "monitor::path for "<<robot_id<< ": "<< newPath << endl;
			//debug code
			publishPath(robot_id,covering,newPath,probs);
			return;
		}
		if(state == covering){//robi finished to cover his area
			vector<pathCell*> path = al->allocateNextArea(c-> getLocation(), robot_id);
			if(path.size() == 0){
				//debug code
				cout<< "monitor::path for "<<robot_id<< ": "<< "end" << endl;
				//debug code
				publishPath(robot_id,done,"end","");
			}
			string newPath = path2str(path);
			string probs = probs2str(path);
			//debug code
			cout<< "monitor::path for "<<robot_id<< ": "<< newPath << endl;
			//debug code
			publishPath(robot_id,traveling,newPath,probs);
			return;
		}
	}
}

void bury(int robot_id){
	/*ids[robot_id] = sekelton;
	for (int i = 0; i < ids.size(); ++i)
	{
		if(ids[robot_id]!= sekelton){return;}
	}
	cout<<"my whole team killed :("<<endl<<"bye..."<<endl;
	exit(0);*/

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

