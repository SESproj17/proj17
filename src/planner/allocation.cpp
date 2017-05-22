
#include "allocation.h"

allocation::allocation(vector<int> givenRobots, vector<myTuple> givenStartLocations){
	team = givenRobots;
	startLocations = givenStartLocations;
    this->a2r = new areas2Robobts();
    assignment = a2r->statrAllocation(startLocations);
    for (int i = 0; i < team.size(); ++i)
    {
    	assignment[i]->changeState(Assigned);
    }
}


void allocation::assign(subArea* area, int robot){
	if(team[robot] == skeleton){cout<<"assign: problem assign to dead robot->EXIT"<<endl; exit(0);}
	if(area->getState() != NotAssigned){cout<<"assign: problem area is Assigned or completed->EXIT"<<endl; exit(0);}
	if(NULL != assignment[robot]){cout<<"assign: problem assign to busy robot->EXIT"<<endl; exit(0);}
	assignment[robot] = area;
	area->changeState(Assigned);
}

void allocation::unAssign(int robot){
	if(robot == skeleton){cout<<"unAssign: problem un-assign to dead robot->EXIT"<<endl; exit(0);}
	subArea* area = assignment[robot];
	if(NULL == area){
		cout<<"unAssign: problem robot is free"<<endl; exit(-1);
	}
	if(area-> getState() != Covered){
		area->changeState(NotAssigned);
	}
	assignment[robot] = NULL;
}

void allocation::bury(int robot_id){
	unAssign(robot_id);
	team[robot_id] = skeleton;
}

vector<pathCell*> allocation::allocateStartArea(int robot_id){
	if(team[robot_id] == skeleton){cout<<"allocateStartArea::allocation to dead robot->EXIT"<<endl;exit(-1);}
	subArea* area = assignment[robot_id];
	return a2r->getSafestPath(startLocations[robot_id], area);
}

vector<pathCell*> allocation::allocateNextArea(myTuple location,int robot_id){
	if(team[robot_id] == skeleton){cout<<"allocateNextArea:: dead robot->EXIT"<<endl;exit(-1);}
	unAssign(robot_id);
	subArea* area = a2r->lookForNewArea(location);
	assign(area, robot_id);
	if(NULL == area){//no more un assigned areas
		vector<pathCell*> empty;
		return empty;
	}
	return a2r->getSafestPath(location,area);
}

/*
greedy approach, where in each step it leads the robot
to the safest nearest cell to its current location which has not been
covered yet.
*/
vector<pathCell*> allocation::areaCoverage(myTuple location, int robot_id){
	subArea* area = assignment[robot_id];
	return area->coverge(location);
}

