#ifndef SUB_AREA
#define SUB_AREA

#include <ros/ros.h>
#include <vector>
#include <iostream>
#include "pathCell.h"
#include "area.h"


using namespace std;
class subArea {
private:
	vector<vector <pathCell*> > myCells;
	vector<pathCell*> cells;
	vector<int> initialRobots;
	int worker;
	float prob;
	int myLevel;
	AreaState state;
	
public:
	subArea(vector<vector <pathCell*> > givenCells,float givenProb,int lvl);
	subArea(vector<pathCell* > newCells,float givenProb,int lvl);
	vector<pathCell*> getCells();
	vector<int> getinitialRobots();
	void addRobot(int robi);
	void setWorker(int robi);
	pathCell* getCellAt(int i,int j);
	float getProb();
	int getLevel();
	void changeState(AreaState newState);
	AreaState getState();
	void print();
};

#endif