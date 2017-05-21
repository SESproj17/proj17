#ifndef AREA_H
#define AREA_H

#include <ros/ros.h>
#include <vector>
#include "pathCell.h"


using namespace std;

enum AreaState {NotAssigned,Assigned,Covered};

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
