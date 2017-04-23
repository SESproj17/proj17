#include "Robot.h"
using namespace std;
#include <stdio.h>

void Robot::work(vector<pathCell*> path){
	robotState = traveling;
	pathCell *nextLocation;
	for (int i = 0; i < path.size() - 1; ++i) {
		location = path[i];
		nextLocation = path[i+1];
		//tryToCover(location);
		location->changeState();
		//moveToNext(location->getLocation(), nextLocation->getLocation());
		
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

	rotate(d);
	step(d);

 }

 void Robot::rotate(Direction d) {
 	ros::Rate rate(40);
	double wantedAngle = 0.0;
	int clock = 1;
	double error = 0.0002;
	boost::shared_ptr<geometry_msgs::PoseWithCovarianceStamped const> position;
	if(d == UP){
		wantedAngle = 1.0;
	}else if(d == LEFT){
		wantedAngle = -0.7071;
	}else if(d == DOWN){
		wantedAngle = 0.0;
	}else{
		wantedAngle = 0.7071;
	}
	position = ros::topic::waitForMessage<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose",(*nh));
	double currentAngle = position->pose.pose.orientation.z;
	//clockwise is negativ movement
	//unclockwise is positive movement
	cout << " need to angle " << wantedAngle << " current angle " << currentAngle << endl;
	while(ros::ok() && abs(wantedAngle - currentAngle) > error){
		if((wantedAngle==1.0 && currentAngle >= 0 &&  currentAngle < 1) ||
		   (wantedAngle == 0.7071 && currentAngle >= -0.7071 && currentAngle < 0.7071) ||
		   (wantedAngle == 0 && currentAngle >= -1 && currentAngle < 0) ||
		   (wantedAngle == -0.7071 && (currentAngle >= 0.7071 || (currentAngle >= -1.0 && currentAngle < -0.7071)))) {
			clock = 1;
			cout<<"clock"<<endl;
		}else{
			clock =-1;
			cout<<"no clock<<"<<endl;
		}
		geometry_msgs::Twist msg; // The default constructor will set all commands to 0
    	msg.angular.z = clock * 0.15;
    	msg.linear.x = 0;
    	publisher.publish(msg);
    	ros::spinOnce();
    	rate.sleep();
    	position = ros::topic::waitForMessage<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose",*nh);
    	currentAngle = position->pose.pose.orientation.z;
    	cout << " need to angle " << wantedAngle << " current angle " << currentAngle << endl;
	}
	
}


 void Robot::step(Direction d){
 	boost::shared_ptr<geometry_msgs::PoseWithCovarianceStamped const> position;
	position = ros::topic::waitForMessage<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose",(*nh));
	cout << position->pose.pose.position.x << " " << position->pose.pose.position.y << " " << position->pose.pose.orientation.z << endl;
	double currentPlace,goal;
	int dir = 1;
	ros::Rate rate(40);
	if (d == UP || d == DOWN) {
		 currentPlace = position->pose.pose.position.x;
		 if (d == UP) {dir = -1;}
		 goal = currentPlace + dir*DX;
		 cout << " need to dist " << goal << " current dist " << currentPlace << endl;
		 while (ros::ok()) {
		 geometry_msgs::Twist msg;
		 	if ((d == UP && goal > currentPlace) || (d == DOWN && goal < currentPlace)) {
		 		msg.linear.x = -0.15;
		 		publisher.publish(msg);
		 		ros::spinOnce();
    			rate.sleep();
		 	} else if ((d == UP && goal < currentPlace) || (d == DOWN && goal > currentPlace)){
		 		msg.linear.x = 0.15;
		 		publisher.publish(msg);
		 		ros::spinOnce();
    			rate.sleep();
		 	}
		 	cout << " need to dist " << goal << " current dist " << currentPlace << endl;
		 	position = ros::topic::waitForMessage<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose",*nh);
		 	currentPlace = position->pose.pose.position.x;
		 }
	} else {
		 currentPlace = position->pose.pose.position.y;
		 if (d == LEFT) { dir = -1;}
		 goal = currentPlace + dir*DX;
		 while (ros::ok()) {
		 	geometry_msgs::Twist msg;
		 	if ((d == RIGHT && goal < currentPlace) || (d == LEFT && goal > currentPlace)) {
		 		msg.linear.x = -0.15;
		 		publisher.publish(msg);
		 		ros::spinOnce();
    			rate.sleep();
		 	} else if ((d == RIGHT && goal > currentPlace) || (d == LEFT && goal < currentPlace)){
		 		msg.linear.x = 0.15;
		 		publisher.publish(msg);
		 		ros::spinOnce();
    			rate.sleep();
		 	}
		 	cout << " need to dist " << goal << " current dist " << currentPlace << endl;
		 	position = ros::topic::waitForMessage<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose",*nh);
		 	currentPlace = position->pose.pose.position.y;
		 }
	}
 }

Robot::Robot(){
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


void Robot::tryToCover(pathCell location){
 	if (location.getProb()<1){
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
/*
void Robot:: action(){
	if(this->robotState == traveling){
		location = nextLocationInPath();
		location->changeState();
		myArea = location->
		goToTheNextCell();
	}
}*/