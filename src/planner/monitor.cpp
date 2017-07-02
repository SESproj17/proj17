
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

enum robotState {idle,traveling,covering,done,dead,deadlock};

int teamSize;
unsigned int robotsCount = 0;
vector<myTuple> locations;
vector<int> team;
vector<bool> lives;
vector<robotState> states;
allocation* al;
bool started = false;//The coverage started
bool end = false;//Coverage should be stopped or completed successfully
vector<bool> deadlocks(2, -1);//robots at deadlock


//team_status topic
ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;

//Subscriber to steps topic
ros::Subscriber steps_sub;

//Publisher to paths topic
ros::Publisher path_pub;

void solveDeadlock(int id);
void coveringPath(int robot_id);
void travelingPath(int robot_id);
void doneCare(int robot_id);
void bury(int id);
string path2str(vector<pathCell*> path);
string probs2str(vector<pathCell*> path);
void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg);
void stepCallback(const ses::step::ConstPtr& step_msg);
void publishPath(int robot_id,robotState state, vector<pathCell*> path);
void handler(int signum);
void checkStates();
bool share(int robot);


int main(int argc, char **argv)
{
	
	ros::init(argc, argv, "monitor");
	ros::NodeHandle nh;

	steps_sub = nh.subscribe("steps", 1, &stepCallback);
	team_status_sub = nh.subscribe("team_status", 1, &teamStatusCallback);

	team_status_pub = nh.advertise<ses::RobotStatus>("team_status", 10);
	path_pub = nh.advertise<ses::Path>("paths", 10);
	
	nh.getParam("group_size",teamSize);

	lives.resize(teamSize);
	for (int i = 0; i < teamSize; ++i)
	{
		lives[i] = false;
	}

	locations.resize(teamSize);
	team.resize(teamSize);
	states.resize(teamSize);

	ROS_INFO("Waiting for robots to connect...");

	while(!end){
		 ros::spinOnce();
	}
	cout<<"finished"<<endl;
}

//alarm handler: Each alert checks the status of the robots on the team if they are still alive
void handler(int signum) {
	//cout<<"monitor::alarm"<<endl;
	//cout<<"monitor:: live robots"<<robotsCount<<endl;
    for (int i = 0; i < lives.size(); ++i)
    {
    	if(!lives[i]){
    		if(states[i]!= dead){
    			robotsCount--;
    			bury(i);
    		}
    	}
    	lives[i] = false;
    }
    if(robotsCount > teamSize || robotsCount <= 0){
    	cout<<"monitor::whole team is dead :("<<endl;
    	grid* g = grid::getInstance();
    	g->print();
    	end = true;
    }else{
    	alarm(12);
    	signal(SIGALRM, handler);
    }
}

void bury(int id){

	//Update the allocation calss
	al->unAssign(id);
	al->bury(id);

	//update grid
	states[id] = dead;
	myTuple place = locations[id];
	grid* g = grid::getInstance();
	cout<<"monitor:robot "<<id<<" died at "<<place.returnFirst()<<","<<place.returnSecond()<<endl;
	pathCell* c = g->getCellAt(place.returnFirst(),place.returnSecond());
	c->setProb(1.0); //No one can pass another cell where a robot has died


	//find replac
	vector<int>free;
	vector<myTuple>locs;
	for (int i = 0; i < states.size(); ++i)
	{
		if(states[i] == done){
			free.push_back(team[i]);
			locs.push_back(locations[i]);
		}
	}
	if(free.size()==0){return;}
	vector<int> reWork = al->restart(free, locs);
	if(reWork.size()==0){
		return;
	}
	for (int i = 0; i < reWork.size(); ++i)
	{
		vector<pathCell*> path = al->allocate(locs[reWork[i]],reWork[i]);
		if(path.size()==1){
			path = al->areaCoverage(locs[reWork[i]],reWork[i]);
			publishPath(reWork[i],covering,path);
		}else{
			publishPath(reWork[i],traveling,path);	
		}
	}
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
				
				
				vector<int> team(teamSize,-1);
				for (int i = 0; i < teamSize; ++i)
				{
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
				status_msg.is_ready = true;
				team_status_pub.publish(status_msg);
			}
		}
	}
}


void publishPath(int robot_id,robotState state, vector<pathCell*>path){

	states[robot_id] = state;

	string probs = "";
	string newPath = "";

	if(state != done){
		 probs = probs2str(path);
		 newPath = path2str(path);
	}

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

void solveDeadlock(int id){
	if(deadlocks[0]== -1){//only one reported about deadlock
		deadlocks[0] = id;
		return;
	}
	deadlocks[1] == id;
	grid* g = grid::getInstance();
	
	pathCell* c = g->getCellAt(locations[id].returnFirst(), locations[id].returnSecond());
	float temp = c->getProb();
	c->setProb(1);
	if(states[id] == traveling){
		travelingPath(id);
	}

	if(states[id] == covering){
		coveringPath(id);
	}
	deadlocks[0] = -1;
	deadlocks[1] = -1;
	c->setProb(temp);	
}

void travelingPath(int robot_id){
	vector<pathCell*> path = al->allocateNextArea(locations[robot_id], robot_id);
	cout<<"monitor::travelingPath:path to new area!"<<path.size()<<endl;
	if(path.size() == 0){
		if(share(robot_id)){return;}
		doneCare(robot_id);
		return;
	}
	publishPath(robot_id,traveling,path);
	return;

}

void coveringPath(int robot_id){
	vector<pathCell*> path = al->areaCoverage(locations[robot_id],robot_id);
	cout<<"monitor::coveringPath path!"<<path.size()<<endl;
	if(path.size() == 0){//area of one cell
		travelingPath(robot_id);
		return;
	}
	publishPath(robot_id,covering,path);
	return;

}

void doneCare(int robot_id){
	vector<pathCell*> path;
	publishPath(robot_id,done,path);
	states[robot_id] = done;
	al->unAssign(robot_id);
	//cout<<"doneCare::done!"<<endl;
	checkStates();
}


//Given the location and state of the robot, the function determines what path the robot must go to, and publishes it
void stepCallback(const ses::step::ConstPtr& step_msg){
	int robot_id = step_msg->robot_id;
	robotState state = (robotState)step_msg->state;

	if(state == deadlock){
		solveDeadlock(robot_id);
		return;
	}

	int x = step_msg->first_location;
	int y = step_msg->second_location;
	myTuple m(x,y);
	locations[robot_id] = m;
	

	states[robot_id] = state;
	cout<<"monitor: "<<robot_id<<"is now at "<<locations[robot_id].returnFirst()<<","<<locations[robot_id].returnSecond()<<endl;
	grid* g = grid::getInstance();
	pathCell* c = g->getCellAt(step_msg->first_location, step_msg->second_location);


	//debug code
	cout<<"monitor:stepCallback::step of robot "<<robot_id<<endl;
	for (int i = 0; i <team.size(); ++i)
	{
		cout<<team[i]<<";";
	}
	cout<<endl;
	for (int i = 0; i <locations.size(); ++i)
	{
		cout<<locations[i].returnFirst()<<","<<locations[i].returnSecond()<<";";
	}
	cout<<endl;
	//debug code

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
	if(state == idle) {
		vector<pathCell*> path = al->allocate(c->getLocation(),robot_id);
		if(path.size()==1){
			coveringPath(robot_id);
			return;
		}
		publishPath(robot_id,traveling,path);
		return;		
	}	
	if(step_msg->is_the_last){//the robot finished his path
		if(state == traveling){//the robot found his area
			coveringPath(robot_id);
			return;
		}
		if(state == covering){//the robot finished to cover his area
			travelingPath(robot_id);
			return;
		}
	}
}

bool share(int robot){
	cout<<robot<<" in share"<<endl;
	
	for (int i = 0;robot!=i && i < states.size(); ++i)
	{
			vector<myTuple> sublocations(2,myTuple(-1,-1));
			sublocations[1] = locations[robot];//helper
			cout<<"loc: "<<sublocations[1].returnFirst()<<","<<sublocations[1].returnSecond()<<endl;
			sublocations[0] = locations[i];//other

		if (i!=robot && (states[i]==covering || states[i]==traveling) && al->isPoint(sublocations,robot,i))
		{
			cout<<"robot "<<robot<<" helps "<<i<<endl;

			
			//other
			vector<pathCell*> otherPath = al->allocate(locations[i],i);
			if(otherPath.size()==1){
				otherPath = al->areaCoverage(locations[i],i);
				publishPath(i,covering,otherPath);
			}else{
				publishPath(i,traveling,otherPath);
			}
			

			//helper
			vector<pathCell*> helperPath = al->allocate(locations[robot],robot);
			publishPath(robot,traveling,helperPath);
			return true;	
		}
	}
	return false;

}

void checkStates(){	
	for (int i = 0; i < states.size(); ++i)
	{
		if (!(states[i]==dead ||states[i]==done))
		{
			cout<<"first..."<<endl;
			return;
		}
	}
	grid* g = grid::getInstance();
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
    g->print();
	cout<<"Coverage End!"<<endl;

	alarm(0);

	//publish a message to all robots that they can close themselfs
	ses::RobotStatus status_msg;
	status_msg.header.stamp = ros::Time::now();
	status_msg.header.frame_id = "monitor";
	status_msg.is_ready = false;
	team_status_pub.publish(status_msg);

	exit(0);
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
		newP +=  ss.str(); 
		if (i!= path.size() -1) {
			newP += " ";
		}
	}
	return newP;

}