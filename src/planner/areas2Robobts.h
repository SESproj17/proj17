#ifndef AREAS2robotS_H
#define AREAS2robotS_H


#include <ros/ros.h>
#include "grid.h"
#include "subArea.h"
#include <vector>
#include "pathCell.h"
#include <algorithm>
#include "costedArea.h"
#include "algo1.h"
#include "splitBetweenRobots.h"
using namespace std;


class areas2Robobts{
private:

	struct cmpr {
	  bool operator() ( costedArea* first,costedArea* second) 
	  { return (first->getCost() < second->getCost());}
	} compByCost;


	const static int D = 4;

	vector<vector<subArea*> > areas;

	void add(subArea* added);
	vector<subArea*> split(vector<int> robots, subArea* areaToSplit);
	vector<subArea*> sortedAvailableAreasPerLocation(myTuple location, AreaState askedState);
	
	
	costedPath* findSafestPath(myTuple robiLocation, subArea* area);
	costedPath* min(vector<costedPath*> paths);
	float price(vector<pathCell*> path);
	
	
public: 
	areas2Robobts();
	vector<subArea*> statrAllocation(vector<myTuple> startLocs);
	vector<pathCell*> getSafestPath(myTuple robiLocation, subArea* area);
	subArea* lookForNewArea(myTuple location);
};


#endif