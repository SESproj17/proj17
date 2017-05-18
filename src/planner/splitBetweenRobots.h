#ifndef SBR_H
#define SBR_H


#include <ros/ros.h>
#include <vector>
#include "pathCell.h"
#include "subArea.h"

using namespace std;
class splitBetweenRobots{
private:

	vector<vector<pathCell*> > graphPartition(vector<pathCell*>graph,int k);
	vector<myTuple*> hungarianMethod(vector<vector<float> >costMatrix);
public:
	splitBetweenRobots(vector<int> robots, subArea* areaToSplit);
	vector<subArea*> split();
};


#endif