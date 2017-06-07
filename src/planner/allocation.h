#ifndef ALOC_H
#define ALOC_H

#include <ros/ros.h>
#include <vector>
#include "pathCell.h"
#include "subArea.h"
#include "areas2Robobts.h"

using namespace std;


//This class is responsible for assigning areas for robots
class allocation{
private:

	const static int skeleton = -1;

	vector<int> team;
	vector<myTuple> startLocations;
	vector<subArea*> assignment;
	areas2Robobts* a2r;//areas finder

	void assign(subArea* area, int robot);
	
	
public:
	allocation(vector<int> givenRobots, vector<myTuple> startLocations);
	void unAssign(int robot);
	vector<pathCell*> allocateStartArea(int robot_id);
	vector<pathCell*> allocateNextArea(myTuple location, int robot_id);
	vector<pathCell*> areaCoverage(myTuple location,int robot_id);
	vector<pathCell*> findAreaToShare(myTuple mylocation, myTuple otherLocation,int myId, int otherId);
};

#endif