#ifndef SUB_AREA
#define SUB_AREA

#include <ros/ros.h>
#include <vector>
#include <iostream>
#include "pathCell.h"
#include "area.h"
class robot;

using namespace std;
class subArea {
private:
	vector<vector <pathCell*> > myCells;
	vector<pathCell*> cells;
	vector<robot*> initialRobots;
	robot* worker;
	float prob;
	int myLevel;
	AreaState state;
	
public:
	subArea(vector<vector <pathCell*> > givenCells,float givenProb,int lvl);
	subArea(vector<pathCell* > newCells,float givenProb,int lvl);
	vector<pathCell*> getCells();
	vector<robot*> getinitialRobots();
	void addRobot(robot* robi);
	void setWorker(robot* robi);
	pathCell* getCellAt(int i,int j);
	float getProb();
	int getLevel();
	void changeState(AreaState newState);
	AreaState getState();
	void print();
};

#endif