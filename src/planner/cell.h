#ifndef CELL_H
#define CELL_H


#include <ros/ros.h>
#include "myTuple.h"
class subArea;

using namespace std;

enum State {Visited = 1,NotVisited = 0};

class cell {

private:
	float prob;
	myTuple* location;
	int id;
	State myState;
	subArea* area;

public:
	cell(myTuple* givenLocation,double givenProb,int givenId) ;
	void setArea(subArea* givenArea);
	float getProb();
	myTuple getLocation();
	bool imAlive();
	int getId();
	bool isEqual(cell* other);
	State getState();
	void changeState();
	void setProb(float p);
};



#endif