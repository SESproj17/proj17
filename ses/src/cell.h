#ifndef CELL_H
#define CELL_H


#include <ros/ros.h>
#include "myTuple.h"
using namespace std;

enum State {Visited = 1,NotVisited = 0};
class cell {

private:
	float prob;
	myTuple* location;
	int id;
	State myState;

public:
	cell(myTuple* givenLocation,float givenProb,int givenId);
	float getProb();
	myTuple* getLocation();
	bool imAlive();
	int getId();
	bool isEqual(cell* other);
	State getState();
	void changeState();
};



#endif