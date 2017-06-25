
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
	if(team[robot] == skeleton){ exit(1);}
	if(area->getState() != NotAssigned){ exit(1);}
	//if(NULL != assignment[robot]){cout<<"assign: problem assign to busy robot->EXIT"<<endl; exit(1);}
	assignment[robot] = area;
	cout<<"robot "<<robot<<endl;
	assignment[robot]->print();
	area->changeState(Assigned);
}

//Taking care of a case where a robot died. Makes sure that the area to which it was allocated is marked as unAssigned
void allocation::unAssign(int robot){
	cout<<"unAssign robot "<<robot<<endl;
	if(team[robot] == skeleton){cout<<"unAssign: robot "<<robot<<" died yesterday!"<<endl;exit(1);}
	subArea* area = assignment[robot];
	if(area-> getState() == NotAssigned){cout<<"unAssign: problem area is not Assigned->EXIT"<<endl; exit(1);}
	if(NULL == area){cout<<"unAssign: problem robot is free"<<endl; exit(1);}
	area->changeState(Covered);
	assignment[robot] = NULL;
	vector<subArea*> newAreas = area->getInheritance();
	//cout<<"unAssign:after getInheritance"<<endl;
	a2r->addSplited(area, newAreas);

}

void allocation::bury(int robot){
	team[robot] = skeleton;
}

vector<pathCell*> allocation::allocate(myTuple location,int robot_id){
	if(team[robot_id] == skeleton){cout<<"allocateStartArea::allocation to dead robot->EXIT"<<endl;exit(1);}
	subArea* area = assignment[robot_id];
	return a2r->getSafestPath(location, area);
}

//Returns the closest safest area to given robot
vector<pathCell*> allocation::allocateNextArea(myTuple location,int robot_id){
	if(team[robot_id] == skeleton){cout<<"allocateNextArea::allocation to dead robot->EXIT"<<endl;exit(1);}
	subArea* area = a2r->lookForNewArea(location);
	
	if(NULL != area){
		assign(area, robot_id);
		return a2r->getSafestPath(location, area);
	}
	vector<pathCell*> empty;
	return empty;
	
}


//Returns a cover path to the robot's area, from its location
vector<pathCell*> allocation::areaCoverage(myTuple location, int robot_id){
	subArea* area = assignment[robot_id];
	return area->coverge(location);
}



vector<int> allocation::restart(vector<int>free,vector<myTuple> locations){
	vector<subArea*> subAssignment = a2r->statrAllocation(locations);
	vector<int> reworkers;
    for (int i = 0; i < free.size(); ++i)
    {
    	assign(subAssignment[i],free[i]);
    	reworkers.push_back(free[i]);

    }
    return reworkers;
}


/*
If the path
from the given robot’s location to the designated area is longer than
the number of unvisited cells in that area, then there is no point of
sending the robot there,
*/ 
bool allocation::isPoint(vector<myTuple>locations, int helper, int other){
	cout<<"helper "<<helper<<" other "<<other<<endl;
	subArea* a = assignment[other];
	safestPath sp(a->getCells());
	vector<pathCell*> way = sp.find(locations[1])->getPath();
	cout<<"a->getLeftCells()"<<a->getLeftCells()<<"   way.size() "<<way.size()<<endl;
	//if(a->getLeftCells()> way.size())
	int x = a->getLeftCells()-way.size();
	cout<<x<<" "<<(x>1)<<endl; 
	if(x>1)
	{
		cout<<"isPoint!"<<endl;
		splitBetweenRobots sbr(a, locations);
		vector<subArea*> splited = sbr.split();
		a2r->addSplited(a,splited);
		assign(splited[1], helper);
		assign(splited[0], other);
		return true;
	}
	cout<<"is no Point"<<endl;
	return false;
}
