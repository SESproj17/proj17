
#include "allocation.h"

allocation::allocation(vector<int> robots, vector<myTuple*> locations){
/*	ros::NodeHandle nh;
	double jump,max;
	nh.getParam("jump_cost",jump);
	nh.getParam("max_cost",max);
    algo1* al = new algo1();
    vector<area*> vc = al->make_areas(jump,max);
    areas = al->getConnectedAreas(vc);
    areas2Robobts* assigner = new areas2Robobts();
    assignment = assigner->statrAllocation(areas,robots,locations);
*/}

void allocation::bury(int robot_id){

}

vector<pathCell*> allocation::allocateNextArea(int robot_id){

}

vector<pathCell*> allocation::areaCoverage(pathCell* c,int robot_id){

}

