/*
 * Robot.h
 *
 */

/*

#ifndef ROBOT_H_
#define ROBOT_H_

#include <ros/ros.h>
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <string.h>
//#include "grid.h"
#include <tf/transform_listener.h>
//#include "area.h"

using namespace std;
enum Direction {RIGHT,UP,LEFT,DOWN};
enum state {idle,traveling,covering,done,dead};

class Robot {
private:
	const static double placeTol = 0.03;
	const static double angularTolerance  = 0.05;
	const static double DX = 0.9;
	const static double DXHorizontal = 1;
	const static double angularSpeed = 0.25;

	double currentLocationX;
	double currentLocationY;
	double currentAngle;

	int newCounter;

	ros::Subscriber poseSubscriber;

	ros::Publisher publisher;
	ros::NodeHandle * nh;
	string robotName;
	state robotState;
	vector<pathCell*> path;
	//area myArea;
	pathCell* location;
	

	void step(Direction d);
	

public:
	void getPose();
	void rotate(Direction d);

	void work(vector<pathCell*> path);
	void moveToNext(myTuple location, myTuple nextLocation);
	void tryToCover(pathCell* location);

	Robot();
	state getState();
	//void set State(state s);
	//area getMyArea();
	//void setArea(area newArea);
	void Move();
	bool haveNoMoreCelles();
	pathCell* getLocation();
	void setPath(std::vector<pathCell*> newpath);
	
};

#endif *//* ROBOT_H_ */
