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
#include "safestPath.h"
using namespace std;

//The class maintains the areas lists and provides an assignment for robots as input
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
	subArea* findAreaToShare();
	
	vector<subArea*> getSafeAreas();
	costedPath* findSafestPath(myTuple robiLocation, subArea* area);
	
	
public: 
	areas2Robobts();
	vector<subArea*> statrAllocation(vector<myTuple> startLocs);
	vector<pathCell*> getSafestPath(myTuple robiLocation, subArea* area);
	subArea* lookForNewArea(myTuple location);
	void addSplited(subArea* oldArea, vector<subArea*> newSplitedAreas);
};


#endif