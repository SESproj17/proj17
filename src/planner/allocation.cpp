
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

//Updating the area the robot is responsible for being the given area
void allocation::assign(subArea* area, int robot){
	if(team[robot] == skeleton){cout<<"assign: problem assign to dead robot->EXIT"<<endl; exit(1);}
	if(area->getState() != NotAssigned){cout<<"assign: problem area is Assigned or completed->EXIT"<<endl; exit(1);}
	//if(NULL != assignment[robot]){cout<<"assign: problem assign to busy robot->EXIT"<<endl; exit(1);}
	assignment[robot] = area;
	assignment[robot]->print();
	area->changeState(Assigned);
}

//Taking care of a case where a robot died. Makes sure that the area to which it was allocated is marked as unAssigned
void allocation::unAssign(int robot){
	if(team[robot] == skeleton){cout<<"unAssign: robot "<<robot<<" died yesterday!"<<endl;return;}
	subArea* area = assignment[robot];
	if(area-> getState() != Assigned){cout<<"unAssign: problem area is not Assigned or completed->EXIT"<<endl; exit(1);}
	if(NULL == area){cout<<"unAssign: problem robot is free"<<endl; exit(1);}
	team[robot] = skeleton;
	area->changeState(Covered);//??????
	assignment[robot] = NULL;
	area->print();
	cout<<"unAssign:before getInheritance"<<endl;
	vector<subArea*> newAreas = area->getInheritance();
	cout<<"unAssign:after getInheritance"<<endl;
	a2r->addSplited(area, newAreas);

}

//Calculates the closest and safest areas for all robots at the start of the algorithm
vector<pathCell*> allocation::allocateStartArea(int robot_id){
	if(team[robot_id] == skeleton){cout<<"allocateStartArea::allocation to dead robot->EXIT"<<endl;exit(1);}
	subArea* area = assignment[robot_id];
	return a2r->getSafestPath(startLocations[robot_id], area);
}

//Returns the closest safest area to given robot
vector<pathCell*> allocation::allocateNextArea(myTuple location,int robot_id){
	if(team[robot_id] == skeleton){cout<<"allocateNextArea::allocation to dead robot->EXIT"<<endl;exit(1);}
	subArea* area = a2r->lookForNewArea(location);
	assign(area, robot_id);
	if(NULL == area){
		vector<pathCell*> empty;
		return empty;
	}
	return a2r->getSafestPath(location, area);
}


//Returns a cover path to the robot's area, from its location
vector<pathCell*> allocation::areaCoverage(myTuple location, int robot_id){
	subArea* area = assignment[robot_id];
	return area->coverge(location);
}

/*
The procedure Find Area To Share
tries to find for a given idle
robot an assigned area that it can help finish covering. If the path
from the given robot’s location to the designated area is longer than
the number of unvisited cells in that area, then there is no point of
sending the robot there, since by the time the robot arrives there, its
coverage will have been completed. If an area that can be shared has
been found, its connected unvisited parts are defined as new areas and
added to the pool of unassigned areas instead of the original area. If
there is only one such part (e.g., its assigned robot has not started
covering it), then it is split into two balanced parts. Finally, both the
assigned robot and the idle robot are (re-)assigned to the sub-areas
with the safest paths from their current location.
*/ 

vector<pathCell*> allocation::findAreaToShare(myTuple mylocation, myTuple otherLocation,int myId, int otherId){
	vector<pathCell*> empty;
	if(!assignment[otherId]){return empty;}
	subArea* a = assignment[otherId];
	if(a->getState() == Covered){return empty;}
	safestPath sp(a->getCells());
	vector<pathCell*> way = sp.find(mylocation)->getPath();
	if(a->getLeftCells()> way.size()){
		vector<myTuple> locations(2,myTuple(-1,-1));
		locations[0] = mylocation;
		locations[1] = otherLocation;
		splitBetweenRobots sbr(a, locations);
		vector<subArea*> splited = sbr.split();
		a2r->addSplited(a,splited);
		return a2r->getSafestPath(mylocation, splited[0]);
	}
	return empty;
}
