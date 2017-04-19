#ifndef SUB_AREA
#define SUB_AREA

#include <ros/ros.h>
#include <vector>
#include <iostream>

enum AreaState {Assigned,NotAssigned,Covered};
using namespace std;
class subArea {
private:
	vector<vector <int> > myCells;
	float prob;
	int myLevel;
	AreaState state;
	//my robots.
public:
	subArea(vector<vector <int> > givenCells,float givenProb,int lvl);
	float getProb();
	int getLevel();
	void changeState(AreaState newState);
	AreaState getState();
};



#endif