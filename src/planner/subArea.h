#ifndef SUB_AREA
#define SUB_AREA

#include <ros/ros.h>
#include <vector>
#include <iostream>
#include "pathCell.h"
#include "area.h"
#include "grid.h"
#include "safestPath.h"
#include <stack>


using namespace std;

//enum AreaState {Assigned,NotAssigned,Covered};

class subArea {
private:
	vector<vector <pathCell*> > myCells;
	vector<pathCell*> cells;
	vector<int> initialRobots;
	int worker;
	float prob;
	int myLevel;
	AreaState state;
	int notFoundYet;
	
public:
	int getLeftCells();
	void print();
	subArea(vector<vector <pathCell*> > givenCells,float givenProb,int lvl);
	subArea(vector<pathCell* > newCells,float givenProb,int lvl);
	vector<pathCell*> getCells();
	vector<pathCell*> coverge(myTuple start);
	vector<int> getinitialRobots();
	void addRobot(int robi);
	void setWorker(int robi);
	pathCell* getCellAt(int i,int j);
	float getProb();
	int getLevel();
	vector<subArea*> dfs();

	void changeState(AreaState newState);
	AreaState getState();
	void notifyVisitedCell();	
};

#endif