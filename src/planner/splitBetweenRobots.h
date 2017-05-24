#ifndef SBR_H
#define SBR_H


#include <ros/ros.h>
#include <vector>
#include "pathCell.h"
#include "hungarian.h"
#include "costedPath.h"
#include "subArea.h"
#include "safestPath.h"

using namespace std;
class splitBetweenRobots{
private:
	subArea* initialArea;
	vector<myTuple> robotLocations;

	vector<vector<pathCell*> > graphPartition(vector<pathCell*>graph,int k);
	vector<vector<pathCell*> > vectorToMatrix(vector<pathCell*> vec);
	int findLocation(pathCell* cell, vector<pathCell*>graph);
	vector<subArea*> convert(vector<vector<int> > HungarianOutput, vector<subArea*>);

public:
	splitBetweenRobots(subArea* areaToSplit, vector<myTuple> locations);
	vector<subArea*> split();
	vector<subArea*> hungarianMethod();
};


#endif