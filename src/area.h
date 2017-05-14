#ifndef AREA_H
#define AREA_H

#include <ros/ros.h>
#include <vector>
#include "pathCell.h"

enum AreaState {Assigned,NotAssigned,Covered};
using namespace std;
class area {
private:
	vector<vector <pathCell*> > myCells;
	float prob;
	int myLevel;
	AreaState state;
	
public:
	area(vector<vector <pathCell*> > givenCells,float givenProb,int lvl);
	float getProb();
	int getLevel();
	pathCell* getCellAt(int i,int j);
	void changeState(AreaState newState);
	AreaState getState();
	bool isCovered();
};

#endif
