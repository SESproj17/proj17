#ifndef AREAS2robotS_H
#define AREAS2robotS_H


#include <ros/ros.h>
#include "grid.h"
#include "subArea.h"
#include <vector>
#include "pathCell.h"
#include "robot.h"
#include <algorithm>
#include "costedArea.h"






class areas2Robobts{
private:
	const static int D = 4;// as the article request
	vector<robot*>team;
	vector<vector<subArea*> >areas;
	vector<subArea*> splitAreaBetweenHisrobots(subArea* area);
	costedPath* min(vector<costedPath*> paths);
	costedPath* findSafestPath(myTuple robiLocation, subArea* area);
	float price(vector<pathCell*> path);
	vector<vector<pathCell*> > graphPartition(vector<pathCell*>graph,int k);
	vector<myTuple*> hungarianMethod(vector<vector<float> >costMatrix);
	vector<subArea*> addAndRemove(vector<subArea*> safests, vector<subArea*>add, vector<int>remove);

	
public: 
	areas2Robobts(vector<vector<subArea*> >givenConnectedAreas,
					vector<robot*>givenTeam);
	void allocate();
	
};

#endif