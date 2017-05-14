	
#ifndef MR_H
#define MR_H

#include <ros/ros.h>
#include <iostream>
#include <string>
#include "ros/ros.h"
#include "robot.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <string.h>
#include <tf/transform_listener.h>
#include <multi_sync/step.h>
#include <multi_sync/Path.h>

using namespace std;
enum Direction {RIGHT,UP,LEFT,DOWN};
enum state {idle,traveling,covering,done,dead};


class moveRobot 
{
public:
	moveRobot(int firstStart, int secondStart, int robot_id);
	void start();
	
private:

	ros::Subscriber poseSubscriber;//??

	ros::Publisher publisher;
	ros::Subscriber path_sub;
	ros::Publisher steps_pub;



	robot* me;
	bool canMove = false;
	int robot_id;


	const static double placeTol = 0.03;
	const static double angularTolerance  = 0.05;
	const static double DX = 0.9;
	const static double DXHorizontal = 1;
	const static double angularSpeed = 0.25;

	double currentLocationX;
	double currentLocationY;
	double currentAngle;

	int newCounter;

	void pathCallback(const multi_sync::Path::ConstPtr& path_msg);
	void publishStep();
	void getPose();
	void rotate(Direction d);
	void step(Direction d);
	void moveToNext(myTuple location, myTuple nextLocation);

};


#endif