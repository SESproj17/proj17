
#include "areas2Robobts.h"
using namespace std;

areas2Robobts::areas2Robobts(){
	ros::NodeHandle nh;
	double jump,max;
	nh.getParam("jump_cost",jump);
	nh.getParam("max_cost",max);
    algo1* al = new algo1();
    vector<area*> vc = al->make_areas(jump,max);
    areas = al->getConnectedAreas(vc);
}

vector<pathCell*> areas2Robobts::getSafestPath(myTuple robiLocation, subArea* area){
	costedPath* cp = findSafestPath(robiLocation,area);
	return cp->getPath();
}

subArea* areas2Robobts::lookForNewArea(myTuple location){
	vector<subArea*> a = sortedAvailableAreasPerLocation(location, NotAssigned);
	cout<<"nuber of safest unassiged areas: "<<a.size()<<endl;
	if(a.size()>0){
		return a[0];
	}
	return NULL;	
}

vector<subArea*> areas2Robobts::statrAllocation(vector<myTuple> teamStartLocations)
{	

	//assign robots in areas
	vector<subArea*> sortedAreas;
	cout<<"statrAllocation::size of teamStartLocations: "<<teamStartLocations.size()<<endl;
	for(int i = 0;i < teamStartLocations.size();i++){	
		cout<<"here"<<endl;
		sortedAreas = sortedAvailableAreasPerLocation(teamStartLocations[i], NotAssigned);
		cout<<"statrAllocation::size of sorted: "<<sortedAreas.size()<<endl;			
		for(int j = 0;j < sortedAreas.size();j++){//check if area is not too dense with robots
			subArea* a = sortedAreas[j];
			if(a->getinitialRobots().size()*D <= a->getCells().size()){// after 
				a->addRobot(i);
				cout<<"statrAllocation::a assigned "<<a->getLevel()<<endl;
				break;
			}
			//cout<<"statrAllocation::need to not beeing here "<<endl;
		}
	}

	vector<subArea*> assignment;
	assignment.resize(teamStartLocations.size());
	
	//we can stop this loop when no robots left
	for(int i = 0;i <sortedAreas.size();i++){
		subArea* a = sortedAreas[i];
		vector<int> idsRobotsOfA= a->getinitialRobots();
		if(idsRobotsOfA.size()>1){
			vector<myTuple> locations(idsRobotsOfA.size(),myTuple(-1,-1));
			for (int i = 0; i < locations.size(); ++i)
			{
				locations[i] = teamStartLocations[i];
			}
			splitBetweenRobots sbr(a, locations);	
			vector<subArea*> splited  = sbr.hungarianMethod();
			addSplited(a,splited);
			for (int i = 0; i < idsRobotsOfA.size(); ++i)
			{
				assignment[idsRobotsOfA[i]] = splited[i];
			}
		} else if (idsRobotsOfA.size()==1){
			int id = a->getinitialRobots()[0];
			assignment[id] = a;
			//cout<<"statrAllocation::a->level: "<<assignment[id]->getLevel()<<endl;
		}
	}
	cout<<"statrAllocation::size of assignment: "<<assignment.size()<<endl;
	return assignment;
}

vector<subArea*> areas2Robobts::sortedAvailableAreasPerLocation(myTuple location, AreaState askedState){
	vector<costedArea*> costedAreas;
	vector<subArea*> safests = getSafeAreas();
	for(int j = 0;j < safests.size();j++){
		subArea* area = safests[j];
		// /area->print();
		if(area->getState() == askedState){
			costedAreas.push_back(new costedArea(area, findSafestPath(location,area)));
		}
	}
	sort (costedAreas.begin(), costedAreas.end(), compByCost);//by cost
	vector<subArea*> sortedAreas;
	for (int i = 0; i < costedAreas.size(); ++i)
	{
		sortedAreas.push_back(costedAreas[i]->getArea());
	}
	return sortedAreas;
}

vector<subArea*> areas2Robobts::getSafeAreas(){
	int i = 0;
	while(areas[i].size() == 0){i++;}
	return areas[i];
}

/*
input: area(oldArea) that splited, new sub areas
the function marked the oldArea as covered and add the new areas to the pool
*/
void areas2Robobts::addSplited(subArea* oldArea, vector<subArea*> newSplitedAreas){
	oldArea->changeState(Covered);
	for (int i = 0; i < newSplitedAreas.size(); ++i)
	{
		add(newSplitedAreas[i]);
	}
}

 costedPath* areas2Robobts::findSafestPath(myTuple robiLocation, subArea* area){
 	vector<pathCell*> cells = area->getCells();
 	safestPath sp(cells);
 	return sp.find(robiLocation);    
 }

 void areas2Robobts::add(subArea* added){
 	int level = added->getLevel();
 	areas[level].push_back(added);
 }
