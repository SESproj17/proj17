
#include "ros/ros.h"
#include <multi_sync/RobotStatus.h>
#include <multi_sync/step.h>
#include <multi_sync/Path.h>
#include "myTuple.h"
using namespace std;

#define MAX_ROBOTS_NUM 20

enum State {idle,traveling,covering,done,dead};

unsigned int teamSize;
unsigned int robotsCount = 0;
bool robotsReady[MAX_ROBOTS_NUM];

//grid* g = grid::getInstance();

ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;

ros::Subscriber steps_sub;
ros::Publisher path_pub;


void teamStatusCallback(const multi_sync::RobotStatus::ConstPtr& status_msg);
void stepCallback(const multi_sync::step::ConstPtr& step_msg);
void publishPath(State state, string path, string area, int robot_id);


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

	team_status_pub = nh.advertise<multi_sync::RobotStatus>("team_status", 10);
	team_status_sub = nh.subscribe("team_status", 1, &teamStatusCallback);

	path_pub = nh.advertise<multi_sync::Path>("paths", 10);
	steps_sub = nh.subscribe("steps", 1, &stepCallback);


	ROS_INFO("Waiting for robots to connect...");

	ros::spin();
}


void teamStatusCallback(const multi_sync::RobotStatus::ConstPtr& status_msg)
{
	int robot_id = status_msg->robot_id;
	if (!robotsReady[robot_id]) {
		ROS_INFO("Robot %d is ready!\n", robot_id);
		robotsReady[robot_id] = true;
		robotsCount++;

		if (robotsCount == teamSize) {
			ROS_INFO("All robots GO!");

			multi_sync::RobotStatus status_msg;

			status_msg.header.stamp = ros::Time::now();
			status_msg.header.frame_id = "monitor";
			team_status_pub.publish(status_msg);
		}
	}
}


void publishPath(int robot_id,State state, string path, string area){
	multi_sync::Path path_msg;

	//path_msg.header.stamp = ros::Time::now();
	path_msg.robot_id = robot_id;
	path_msg.path = path;
	path_msg.state = state;
	path_msg.area = area;
	
	sleep(1.0);
	path_pub.publish(path_msg);
}


void stepCallback(const multi_sync::step::ConstPtr& step_msg){

	cout<<"monitor catched a step"<<endl;

	int robot_id = step_msg->robot_id;
	State state = (State)step_msg->state;
	myTuple location(step_msg->first_location, step_msg->first_location);

	//publish only if the status changed!
	if(state == idle || state == done){
		//allocateNextArea(?);
		string newArea = "a area";
		string newPath = "0,0 0,1 1,1 1,2 2,2 2,3 3,3";
		//string newPath = makePath(g->dijkstra(location.returnFirst(),location.returnSecond()));
		publishPath(robot_id,traveling,newPath,newArea);
	}/*else{
		markAsVisited(location);
		if(hitBy(location)){
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
				publishPath(robot_id,covering,areaCoverage(location,step_msg->area),step_msg->area);
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
	}	*/
}

//string makePath(vector<pathCell*>){
//	return ;
//}