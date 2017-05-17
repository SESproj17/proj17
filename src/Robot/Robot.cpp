#include "Robot.h"
using namespace std;
#include <stdio.h>

/*
void Robot::work(vector<pathCell*> path){
	robotState = traveling;
	reverse(path.begin(),path.end());
	pathCell *nextLocation;
	for (int i = 0; i < path.size() - 1; ++i) {
		location = path[i];
		nextLocation = path[i+1];
		tryToCover(location);
		location->changeState();
		moveToNext(location->getLocation(), nextLocation->getLocation());
		
	}

	location = path[path.size() - 1];
	location->changeState();
}

 void Robot::moveToNext(myTuple location, myTuple nextLocation){
 	Direction d;
 	if (location.returnFirst() == nextLocation.returnFirst() && location.returnSecond() > nextLocation.returnSecond()) {
 		d = LEFT;
	} else if (location.returnFirst() == nextLocation.returnFirst() && location.returnSecond() < nextLocation.returnSecond()) {
		d = RIGHT;
	} else if (location.returnFirst() > nextLocation.returnFirst() && location.returnSecond() == nextLocation.returnSecond()) {
		d = UP;	
	} else if (location.returnFirst() < nextLocation.returnFirst() && location.returnSecond() == nextLocation.returnSecond()) {
		d = DOWN;						
	}
	cout << "DIR " << d <<endl;
	rotate(d);
	step(d);

 }

 void Robot::rotate(Direction d) {

	double directionAngles[] = { 0, M_PI / 2, M_PI, -M_PI/2 };
	double targetAngle = directionAngles[d];
	bool turnLeft;
	getPose();
	cout << "current " << currentAngle << endl;
	// Decide to which side to rotate - left or right by choosing the small angle
	// bool turnLeft;
	if (targetAngle - currentAngle > 0 && targetAngle - currentAngle < M_PI)
		turnLeft = true;
	else if (targetAngle - currentAngle < -M_PI)
		turnLeft = true;
	else
		turnLeft = false;


	geometry_msgs::Twist rotateCommand;
	rotateCommand.angular.z = turnLeft ? angularSpeed : -angularSpeed;

	// How fast will we update the robot's movement
	ros::Rate rate(50);

	// Rotate until the robot reaches the target angle
	while (ros::ok() && abs(currentAngle - targetAngle) > angularTolerance * 50) {

		// The robot can reach the LEFT direction from negative PI or positive PI
		if (d == LEFT && (abs(currentAngle - (-M_PI)) <= angularTolerance * 50))
		    break;

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();
	}
	cout << "Angle refinement #1" << endl;
	// Slow the speed near the target
	//rotateCommand.angular.z = turnLeft ? 0.1 * angularSpeed : -0.1 * angularSpeed;
	rotateCommand.angular.z = turnLeft ? 0.5 * angularSpeed : -0.5 * angularSpeed;

	while (ros::ok() && abs(currentAngle - targetAngle) > angularTolerance * 10) {
		// The robot can reach the LEFT direction from negative PI or positive PI
		if (d == LEFT && (abs(currentAngle - (-M_PI)) <= angularTolerance * 10))
			break;
			

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();

		
	}
	cout << "Angle refinement #2" << endl;
	// Further refine the angle
	//rotateCommand.angular.z = turnLeft ? 0.05 * angularSpeed : -0.05 * angularSpeed;
	while (ros::ok() && abs(currentAngle - targetAngle) > angularTolerance) {
		// The robot can reach the LEFT direction from negative PI or positive PI
		if (d == LEFT && (abs(currentAngle - (-M_PI)) <= angularTolerance))
			break;
		

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();


	}
	cout << "Angle refinement #3" << endl;


	for (int i = 0; i < 20; i++) {
		geometry_msgs::Twist stopCommand;
		stopCommand.angular.z = 0;
		publisher.publish(stopCommand);
	}
	
}


 void Robot::step(Direction d){
 	getPose();
	double currentPlace,goal;
	int dir = 1;
	ros::Rate rate(40);
	if (d == UP || d == DOWN) {
		 currentPlace = currentLocationY;
		 if (d == DOWN) {dir = -1;}
		 goal = currentPlace + dir*DX;

		 while (ros::ok()) {
		 	if (abs(currentLocationY - goal) < placeTol) {break;}
		 	geometry_msgs::Twist msg;
		 	if (d == DOWN) {
		 		msg.linear.x = 0.2;
		 	} else {
		 		msg.linear.x = -0.2;
		 	}
		 	cout << " need to dist " << goal << " current dist " << currentLocationY << endl;
		 	publisher.publish(msg);
		 	getPose();
		 }
	} else {
		 getPose();
		 if (d == LEFT) { dir = -1;}
		 goal = currentLocationX + dir*DXHorizontal;
		 while (ros::ok()) {
		 	if (abs(currentLocationX - goal) < placeTol) {break;}
		 	geometry_msgs::Twist msg;
		 	if (d == RIGHT) {
		 		msg.linear.x = 0.2;
		 	} else {
		 		msg.linear.x = -0.2;
		 	}
		 	cout << " need to dist " << goal << " current dist " << currentLocationX << endl;
		 	publisher.publish(msg);		 	
		 	getPose();
		 }
	}
	for (int i = 0; i < 20; i++) {
		geometry_msgs::Twist stopCommand;
		stopCommand.angular.z = 0;
		publisher.publish(stopCommand);
	}
 }

Robot::Robot(){
	ros::NodeHandle  nh;
	publisher  = nh.advertise<geometry_msgs::Twist>("/lizi_1/diff_driver/command", 10);
	robotState = idle;
	location = NULL;
}

state Robot::getState() { return this->robotState;}
void setState(state s);
//area getArea();
//void setArea(Area area);
// bool hittedBy(pathCell c);//ask shalevs cell for p
// bool hasNoMoreCelles();
// pathCell* getLocation();
// void setPath(std::vector<pathCell> path);
// void die();

// Robot::Robot(area myArea, String name): area(myArea), robotName(name), robotState(idle) {
// 	planner = //creat planner...connect to topic of shered mm	
// 	planner.notify(robotName + " was born!");
// 	}		


void Robot::tryToCover(pathCell* location){
 	if (location->getProb()<1){
 		//update the Tau:
 		//planner.notify(""+location.id() +"covered successfully");
 		return;
 	}
 	exit(1);
 }

// Area Robot::getArea(){
// 	return area;
// }
//

bool Robot::haveNoMoreCelles(){
	return path.empty();
}

pathCell* Robot::getLocation(){
	return location;
}

void Robot::setPath(std::vector<pathCell*> newpath){
	path = newpath;
}

void Robot::getPose() {
	tf::TransformListener listener;
	tf::StampedTransform transform;

	listener.waitForTransform("/map", "/lizi_1/base_link", ros::Time(0), ros::Duration(10.0) );

    try {
    	//listener.waitForTransform("/map", "/" + robotName + "/base_link", ros::Time(0), ros::Duration(60.0));
        listener.lookupTransform("/map",  "/lizi_1/base_link", ros::Time(0), transform);
        currentLocationX = transform.getOrigin().x();
        currentLocationY = transform.getOrigin().y();
        currentAngle = tf::getYaw(transform.getRotation());
    }
    catch (tf::TransformException & ex) {
        ROS_ERROR("%s", ex.what());
    } 
}*/