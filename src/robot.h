	
#ifndef R_H
#define R_H

#include <ros/ros.h>
#include <iostream>
#include <string>
#include "myTuple.h"

using namespace std; 

enum State {idle,traveling,covering,done,dead};

class robot 
{
public:
	robot(int first, int second);
	void setPath(string path);
	void setState(State state);
	void setArea(string area);
	State getState();
	myTuple getLocation();
	bool isTheLast();
	string getArea();
private:
	myTuple location;
	vector <myTuple*> path; 
	State state;
	string area;

	vector<string> split(const string &s, char delim);
	int string2int(string s);
	
};


#endif