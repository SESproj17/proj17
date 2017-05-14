
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

#include <string>
#include <vector>

#include <multi_sync/RobotStatus.h>
#include <multi_sync/step.h>
#include <multi_sync/Path.h>
#include "myTuple.h"
#include "robot.h"

using namespace std;

#define MAX_ROBOTS_NUM 20
#define FORWARD_SPEED 0.2
#define MIN_SCAN_ANGLE -60.0/180*M_PI
#define MAX_SCAN_ANGLE +60.0/180*M_PI
#define MIN_PROXIMITY_RANGE 0.5

int robot_id;
bool keepMoving = true;
bool teamReady = false;


ros::Publisher cmd_vel_pub; // publisher for movement commands
ros::Subscriber laser_scan_sub; // subscriber to the robot's laser scan topic

ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;


ros::Subscriber path_sub;
ros::Publisher steps_pub;

robot* me;
	 

void publishReadyStatus();
void publishStep();
void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
void teamStatusCallback(const multi_sync::RobotStatus::ConstPtr& status_msg);
void pathCallback(const multi_sync::Path::ConstPtr& path_msg);
void move_forward();



int main(int argc, char **argv)
{

	if (argc < 2) {
		ROS_ERROR("You must specify robot id.");
		return -1;
	}

	char *id = argv[1];
	robot_id = atoi(id);

	char *x = argv[2];
	int first = atoi(x);

	char *y = argv[3];
	int second = atoi(y);

	myTuple m(first, second);
	me = new robot(m);
	//robot me(m);

	// Check that robot id is between 0 and MAX_ROBOTS_NUM
	if (robot_id > MAX_ROBOTS_NUM || robot_id < 0 ) {
	    ROS_ERROR("The robot's ID must be an integer number between 0 an 19");
	    return -1;
	}
	ROS_INFO("Starting robot %d", robot_id);

	// Create a unique node name
	string node_name = "move_robot_";
	node_name += id;

	ros::init(argc, argv, node_name);
	ros::NodeHandle nh;


	// cmd_vel_topic = "robot_X/cmd_vel"
	string cmd_vel_topic_name = "robot_";
	cmd_vel_topic_name += id;
	cmd_vel_topic_name += "/cmd_vel";
	cmd_vel_pub = nh.advertise<geometry_msgs::Twist>(cmd_vel_topic_name, 10);


	// subscribe to robot's laser scan topic "robot_X/base_scan"
	///string laser_scan_topic_name = "robot_";
	///laser_scan_topic_name += id;
	///laser_scan_topic_name += "/base_scan";
	///laser_scan_sub = nh.subscribe(laser_scan_topic_name, 1, &scanCallback);
	

	// Publish and subscribe to team status messages
	team_status_pub = nh.advertise<multi_sync::RobotStatus>("team_status", 10);
	team_status_sub = nh.subscribe("team_status", 1, &teamStatusCallback);

	// Publish and subscribe to steps and Paths messages
	steps_pub = nh.advertise<multi_sync::step>("steps", 10);
	path_sub = nh.subscribe("paths", 1, &pathCallback);


	publishReadyStatus();

	waitForTeam();

	move_forward();
	return 0;
}


void pathCallback(const multi_sync::Path::ConstPtr& path_msg){
	if(robot_id == path_msg->robot_id){
		me->setState((State)path_msg->state);
		me->setPath(path_msg->path);
		me->setArea(path_msg->area);

	}
}

void teamStatusCallback(const multi_sync::RobotStatus::ConstPtr& status_msg)
{
	// Check if message came from monitor
	if (status_msg->header.frame_id == "monitor") {
		ROS_INFO("Robot %d: Team is ready. Let's move!", robot_id);
		teamReady = true;
	}
}


// Process the incoming laser scan message
void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	// Find the closest range between the defined minimum and maximum angles
	int minIndex = ceil((MIN_SCAN_ANGLE - scan->angle_min) / scan->angle_increment);
	int maxIndex = floor((MAX_SCAN_ANGLE - scan->angle_min) / scan->angle_increment);

	float closestRange = scan->ranges[minIndex];
	for (int currIndex = minIndex + 1; currIndex <= maxIndex; currIndex++) {
		if (scan->ranges[currIndex] < closestRange) {
			closestRange = scan->ranges[currIndex];
		}
	}

	//ROS_INFO_STREAM("Closest range: " << closestRange);

	if (closestRange < MIN_PROXIMITY_RANGE) {
		keepMoving = false;
	}
}



void publishReadyStatus()
{
	multi_sync::RobotStatus status_msg;

	status_msg.header.stamp = ros::Time::now();
	status_msg.robot_id = robot_id;
	status_msg.is_ready = true;

	// Wait for the publisher to connect to subscribers
	sleep(1.0);
	team_status_pub.publish(status_msg);

	ROS_INFO("Robot %d published ready status", robot_id);
}

void waitForTeam()
{
	ros::Rate loopRate(1);

	// Wait until all robots are ready...
	while (!teamReady) {
		ROS_INFO("Robot %d: waiting for team", robot_id);
		ros::spinOnce();
		loopRate.sleep();
	}
}


void move_forward()
{
	// Drive forward at a given speed.
	geometry_msgs::Twist cmd_vel;
	cmd_vel.linear.x = FORWARD_SPEED;
	cmd_vel.angular.z = 0.0;

	// Loop at 10Hz, publishing movement commands until we shut down
	ros::Rate rate(10);

	while (ros::ok() && keepMoving) // Keep spinning loop until user presses Ctrl+C
	{

		cmd_vel_pub.publish(cmd_vel);
		publishStep();
		//me.movement();
		ros::spinOnce(); // Need to call this function often to allow ROS to process incoming messages
		rate.sleep();
	}

	// Stop the robot
	geometry_msgs::Twist stop_cmd_vel;
	stop_cmd_vel.linear.x = 0.0;
	stop_cmd_vel.angular.z = 0.0;
	cmd_vel_pub.publish(stop_cmd_vel);

	ROS_INFO("robot no. %d stopped", robot_id);
}


void publishStep(){
	
	multi_sync::step step_msg;

	step_msg.robot_id = robot_id;
	step_msg.state = me->getState();
	step_msg.first_location = me->getLocation().returnFirst();
	step_msg.second_location = me->getLocation().returnSecond();
	step_msg.is_the_last = me->isTheLast();
	step_msg.area = me->getArea();
	
	sleep(1.0);
	steps_pub.publish(step_msg);
	ros::spinOnce();
	
	ROS_INFO("Robot %d published on %d,%d step", robot_id, step_msg.first_location, step_msg.second_location);
}