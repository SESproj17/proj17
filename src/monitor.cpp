
#include "ros/ros.h"
#include <ses/RobotStatus.h>
#include <ses/step.h>
#include "algo1.h"
#include <ses/Path.h>
#include "grid.h"
#include "myTuple.h"
//#include "areas2Robobts.h"
#include "costedArea.h"
using namespace std;

#define MAX_ROBOTS_NUM 20

enum robotState {idle,traveling,covering,done,dead};

unsigned int teamSize;
unsigned int robotsCount = 0;
bool robotsReady[MAX_ROBOTS_NUM];
vector<int> team;
map<int, costedArea*> assignment;


ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;

ros::Subscriber steps_sub;
ros::Publisher path_pub;

void preProssesing();
void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg);
void stepCallback(const ses::step::ConstPtr& step_msg);
void publishPath(robotState state, string path, string area, int robot_id);


int main(int argc, char **argv)
{
	 cout<<"are you dying???"<<endl;
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

	preProssesing();
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
	if (!robotsReady[robot_id]) {
		ROS_INFO("Robot %d is ready!\n", robot_id);
		robotsReady[robot_id] = true;
		robotsCount++;
		team.push_back(robot_id);

		if (robotsCount == teamSize) {
			ROS_INFO("All robots GO!");

			ses::RobotStatus status_msg;

			status_msg.header.stamp = ros::Time::now();
			status_msg.header.frame_id = "monitor";
			team_status_pub.publish(status_msg);
		}
	}
}


void publishPath(int robot_id,robotState state, string path, string area){
	ses::Path path_msg;

	//path_msg.header.stamp = ros::Time::now();
	path_msg.robot_id = robot_id;
	path_msg.path = path;
	path_msg.state = state;
	path_msg.area = area;
	
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
	if(state == idle || state == done){
		//allocateNextArea(?);
		string newArea = "a area";
		string newPath = "0,0 0,1 1,1 1,2 2,2 2,3 3,3";
		publishPath(robot_id,traveling,newPath,newArea);
	}else{
		c->changeState();
		/*
		if(hitBy(c)){
			bury(robot_id);//?
			a->setState(unAssinged);
			reallocate(step_msg->area);
			publishPath(robot_id,dead, NULL,NULL);
			//return;
		}
		if(state == traveling){
			subArea* a;// = area that contain c(current cell)
			if(/*all cells in a covered*//*){
				a->setState(completed);
			}
			if(step_msg->is_the_last){ //robi found his area
				publishPath(robot_id,covering,areaCoverage(c,step_msg->area),step_msg->area);
			}
		}else if(state == covering){
			if(step_msg->is_the_last){//robi finished to cover his area
				a->setState(completed);
				allocateNextArea(?);
				newArea
				newPath
				publishPath(robot_id,travling,newPath,newArea);
			}
		}
	*/}
}

/*
bool hitBy(pathCell* location){//??????????????????
	grid* g = grid::getInstance();
	pathCell* c = g->getCellAt(location->returnFirst(),location->returnSecond());
	return c->getProb()<1;//?
}*/

void preProssesing(){
    algo1* al = new algo1();
    vector<area*> vc = al->make_areas(0.1,0.4);
    vector<vector<subArea*> > subAreas = al->getConnectedAreas(vc);
    areas2Robobts* assigner = new areas2Robobts(subAreas, team);
    assignment = assigner.allocate();
}


