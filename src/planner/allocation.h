#ifndef ALOC_H
#define ALOC_H

#include <ros/ros.h>
#include <vector>
#include "pathCell.h"
//#include "subArea.h"
//#include "area.h"
//#include "grid.h"
//#include "areas2Robobts.h"

using namespace std;

class allocation{
private:
	
public:
	allocation(vector<int> robots, vector<myTuple*> locations);
	void bury(int robot_id);
	vector<pathCell*> allocateNextArea(int robot_id);
	vector<pathCell*> areaCoverage(pathCell* c,int robot_id);
};

#endif