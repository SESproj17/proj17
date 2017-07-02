	
#ifndef R_H
#define R_H

#include <ros/ros.h>
#include <iostream>
#include <string>
#include "myTuple.h"

using namespace std; 

enum robotState {idle,traveling,covering,done,dead,deadlock};

// The class holds information about the robot
class robot 
{
public:
	robot(int first, int second);
	void setPath(string path);
	void setProbs(string probs);
	void setState(robotState state);
	void setLocation(myTuple* newTuple);
	robotState getState();
	myTuple* getLocation();
	vector<myTuple*> getPath();
	bool isTheLast();
	float getProb();
	bool imAlive();
	void move();

private:
	myTuple* location;
	vector<myTuple*> path; 
	robotState state;
	vector<float> probs;
	
	vector<string> split(const string &s, char delim);
	int string2int(string s);

	
};


#endif