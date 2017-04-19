/*
 * Robot.h
 *
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <ros/ros.h>
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <string.h>
#include "grid.h"
using namespace std;
enum Direction {DOWN,RIGHT,UP,LEFT};
enum state {idle,traveling,covering,done,dead};
class Robot {
private:
	const static double DX = 0.47;
	ros::Publisher publisher;
	ros::NodeHandle * nh;
	//ros::NodeHandle nh;
	string robotName;
	state robotState;
	vector<pathCell*> path;
	//Area area;
	pathCell* location;
	//topic planner
	void rotate(Direction d);
	void step(Direction d);
	

public:
	void work(vector<pathCell*> path);
	 void moveToNext(myTuple location, myTuple nextLocation);
	 void tryToCover(pathCell location);

	Robot();//create a robot
	state getState();
	//void set State(state s);
	//Area getArea();
	//void setArea(Area area);
	void Move();//shalevs code
	//bool hittedBy(pathCell* c);//ask shalevs cell for p
	bool haveNoMoreCelles();
	pathCell* getLocation();
	void setPath(std::vector<pathCell*> newpath);
	//void die();
	//void notify();
	
};

#endif /* ROBOT_H_ */