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
	vector<pathCell*> findAreaToShare(myTuple mylocation, myTuple otherLocation,int myId, int otherId);
	
	
public:
	allocation(vector<int> givenRobots, vector<myTuple> startLocations);
	void unAssign(int robot);
	void bury(int robot);
	bool isPoint(vector<myTuple>locations, int helper, int other);
	vector<int> restart(vector<int>free,vector<myTuple> locations);
	vector<pathCell*> allocate(myTuple location,int robot_id);
	vector<pathCell*> allocateNextArea(myTuple location, int robot_id);
	vector<pathCell*> areaCoverage(myTuple location,int robot_id);
	
};

#endif