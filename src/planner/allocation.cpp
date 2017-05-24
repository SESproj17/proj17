
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
	if(robot == skeleton){cout<<"assign: problem assign to dead robot->EXIT"<<endl; exit(0);}
	if(area->getState() != NotAssigned){cout<<"assign: problem area is Assigned or completed->EXIT"<<endl; exit(0);}
	if(NULL != assignment[robot]){cout<<"assign: problem assign to busy robot->EXIT"<<endl; exit(0);}
	assignment[robot] = area;
	area->changeState(Assigned);
}

void allocation::unAssign(int robot){
	if(robot == skeleton){cout<<"unAssign: problem un-assign to dead robot->EXIT"<<endl; exit(0);}
	subArea* area = assignment[robot];
	if(area-> getState() != Assigned){cout<<"unAssign: problem area is not Assigned or completed->EXIT"<<endl; exit(0);}
	if(NULL == area){cout<<"unAssign: problem robot is free"<<endl; exit(0);}
	area->changeState(NotAssigned);
	assignment[robot] = NULL;
	vector<subArea*> newAreas = area->dfs();
	a2r->addSplited(area, newAreas);

}

void allocation::bury(int robot_id){
	unAssign(robot_id);
	team[robot_id] = skeleton;
}

vector<pathCell*> allocation::allocateStartArea(int robot_id){
	if(robot_id == skeleton){cout<<"allocateStartArea::allocation to dead robot->EXIT"<<endl;exit(0);}
	subArea* area = assignment[robot_id];
	return a2r->getSafestPath(startLocations[robot_id], area);
}

vector<pathCell*> allocation::allocateNextArea(myTuple location,int robot_id){
	if(robot_id == skeleton){cout<<"allocateNextArea::allocation to dead robot->EXIT"<<endl;exit(0);}
	subArea* area = a2r->lookForNewArea(location);
	assign(area, robot_id);
	if(NULL == area){
		vector<pathCell*> empty;
		return empty;
	}
	return a2r->getSafestPath(location, area);
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
