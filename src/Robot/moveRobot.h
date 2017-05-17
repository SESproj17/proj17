	
#ifndef MR_H
#define MR_H

#include <ros/ros.h>
#include <iostream>
#include <string>
#include "ros/ros.h"
#include "robot.h"
//#include "../planner/myTuple.h"
#include "myTuple.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <string.h>
#include <tf/transform_listener.h>
#include <ses/step.h>
#include <ses/Path.h>

using namespace std;
enum Direction {RIGHT,UP,LEFT,DOWN};


class moveRobot 
{
public:
	moveRobot(int firstStart, int secondStart, int robot_id);
	void start();
	
private:

	ros::Subscriber poseSubscriber;

	ros::Publisher publisher;
	ros::Subscriber path_sub;
	ros::Publisher steps_pub;
	ros::Subscriber laserSub;



	robot* me;
	bool canMove;
	int robot_id;
	

	const static double MIN_SCAN_ANGLE = -30.0/180*M_PI;
    const static double MAX_SCAN_ANGLE = +30.0/180*M_PI;
    const static float MIN_DIST_FROM_OBSTACLE = 0.8;
	const static double placeTol = 0.03;
	const static double angularTolerance  = 0.05;
	const static double DX = 0.9;
	const static double DXHorizontal = 1;
	const static double angularSpeed = 0.25;

	double currentLocationX;
	double currentLocationY;
	double currentAngle;

	int newCounter;

	void pathCallback(const ses::Path::ConstPtr& path_msg);
	void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
	void publishStep();
	void getPose();
	void rotate(Direction d);
	void step(Direction d);
	void moveToNext(myTuple* location, myTuple* nextLocation);

};


#endif