
#include "allocation.h"

allocation::allocation(vector<int> givenRobots, vector<myTuple> givenStartLocations){
	team = givenRobots;
	startLocations = givenStartLocations;
    this->a2r = new areas2Robobts();
    assignment = a2r->statrAllocation(startLocations);
    for (int i = 0; i < team.size(); ++i)
    {
    	assign(assignment[i],team[i]);
    }
}


void allocation::assign(subArea* area, int robot){
	if(robot == skeleton){cout<<"assign: problem assign to dead robot"<<endl; exit(0);}
	if(area->getState() != NotAssigned){cout<<"assign: problem area is Assigned or completed"<<endl; exit(0);}
	if(NULL != assignment[robot]){cout<<"assign: problem assign to busy robot"<<endl; exit(0);}
	assignment[robot] = area;
	area->changeState(Assigned);
}

void allocation::unAssign(int robot){
	if(robot == skeleton){cout<<"unAssign: problem un-assign to dead robot"<<endl; exit(0);}
	subArea* area = assignment[robot];
	if(area-> getState() != Assigned){cout<<"unAssign: problem area is not Assigned or completed"<<endl; exit(0);}
	if(NULL == area){cout<<"unAssign: problem robot is free"<<endl; exit(0);}
	area->changeState(NotAssigned);
	assignment[robot] = NULL;

}

void allocation::bury(int robot_id){
	unAssign(robot_id);
	team[robot_id] = skeleton;
}

vector<pathCell*> allocation::allocateStartArea(int robot_id){
	if(robot_id == skeleton){cout<<"allocateStartArea::allocation to dead robot"<<endl;exit(0);}
	subArea* area = assignment[robot_id];
	return a2r->getSafestPath(startLocations[robot_id], area);
}

vector<pathCell*> allocation::allocateNextArea(myTuple location,int robot_id){
	if(robot_id == skeleton){cout<<"allocateNextArea::allocation to dead robot"<<endl;exit(0);}
	subArea* area = a2r->lookForNewArea(location);
	assign(area, robot_id);
	return a2r->getSafestPath(location,area);
}

vector<pathCell*> allocation::areaCoverage(myTuple location, int robot_id){

}

