	
#ifndef R_H
#define R_H

#include <ros/ros.h>
#include <iostream>
#include <string>
#include "subArea.h"

using namespace std; 

enum robotState {idle,traveling,covering,done,dead};

class robot 
{
public:
	robot(int first, int second);
	void setStrPath(string path);
	void setPath(vector<pathCell*> path);
	void setState(robotState state);
	void setStrArea(string area);
	void setArea(subArea*);
	void setLocation(myTuple* newTuple);
	robotState getState();
	myTuple* getLocation();
	vector<myTuple*> getPath();
	bool isTheLast();
	string getArea();
private:
	myTuple* location;
	vector<myTuple*> path; 
	robotState state;
	string area;

	vector<string> split(const string &s, char delim);
	int string2int(string s);
	
};


#endif