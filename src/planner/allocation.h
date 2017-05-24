#ifndef ALOC_H
#define ALOC_H

#include <ros/ros.h>
#include <vector>
#include "pathCell.h"
#include "subArea.h"
#include "areas2Robobts.h"

using namespace std;

class allocation{
private:

	const static int skeleton = -1;

	vector<int> team;
	vector<myTuple> startLocations;
	vector<subArea*> assignment;
	areas2Robobts* a2r;

	void assign(subArea* area, int robot);
	void unAssign(int robot);

	
public:
	allocation(vector<int> givenRobots, vector<myTuple> startLocations);
	void bury(int robot_id);
	vector<pathCell*> allocateStartArea(int robot_id);
	vector<pathCell*> allocateNextArea(myTuple location, int robot_id);
	vector<pathCell*> areaCoverage(myTuple location,int robot_id);
	vector<pathCell*> findAreaToShare(myTuple mylocation, myTuple otherLocation,int myId, int otherId);
};

#endif