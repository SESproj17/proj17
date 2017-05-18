
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
vector<int> ids;
vector<myTuple> locs;
allocation* al;


ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;

ros::Subscriber steps_sub;
ros::Publisher path_pub;

void preProssesing();
string path2str(vector<pathCell*> path);
void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg);
void stepCallback(const ses::step::ConstPtr& step_msg);
void publishPath(robotState state, string path, string area, int robot_id);


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


void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg)
{
	int robot_id = status_msg->robot_id;
	int x = status_msg->start_x;
	int y = status_msg->start_y;
	if (!robotsReady[robot_id]) {
		ROS_INFO("Robot %d is ready!\n", robot_id);
		robotsReady[robot_id] = true;
		robotsCount++;
		ids.push_back(robot_id);
		cout<<"callBack: number of robots: "<<ids.size()<<endl;
		myTuple m(x,y);
		locs.push_back (m);
		
		if (robotsCount == teamSize) {
			ROS_INFO("All robots GO!");

		 	al = new allocation(ids,locs);

			ses::RobotStatus status_msg;
			status_msg.header.stamp = ros::Time::now();
			status_msg.header.frame_id = "monitor";
			team_status_pub.publish(status_msg);
		}
	}
}


void publishPath(int robot_id,robotState state, string path){
	ses::Path path_msg;

	//path_msg.header.stamp = ros::Time::now();
	path_msg.robot_id = robot_id;
	path_msg.path = path;
	path_msg.state = state;
	
	sleep(1.0);
	path_pub.publish(path_msg);
}


void stepCallback(const ses::step::ConstPtr& step_msg){

	cout<<"monitor catched a step"<<endl;

	int robot_id = step_msg->robot_id;
	robotState state = (robotState)step_msg->state;
	grid* g = grid::getInstance();
	pathCell* c = g->getCellAt(step_msg->first_location, step_msg->first_location);
	//publish only if the status changed!
	if(state == idle || state == done){//done-??????
		string newPath = path2str(al->allocateStartArea(robot_id));
		publishPath(robot_id,traveling,newPath);
		return;
	}
	c->changeState();
	if(!(c->imAlive())){
		//set c as obstacle
		al->bury(robot_id);
		publishPath(robot_id,dead, "wait for Resurrection");
		return;
	}

	if(step_msg->is_the_last){
		if(state == traveling){//robi found his area
			string newPath = path2str(al->areaCoverage(c->getLocation(),robot_id));
			publishPath(robot_id,covering,newPath);
			return;
		}
		if(state == covering){//robi finished to cover his area
			string newPath = path2str(al->allocateNextArea(c-> getLocation(), robot_id));
			publishPath(robot_id,traveling,newPath);
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


