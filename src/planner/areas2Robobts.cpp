
#include "areas2Robobts.h"
using namespace std;

areas2Robobts::areas2Robobts(){
	ros::NodeHandle nh;
	//create the areas
	double jump,max;
	nh.getParam("jump_cost",jump);
	nh.getParam("max_cost",max);
    algo1* al = new algo1();
    vector<area*> vc = al->make_areas(jump,max);
    for (int i = 0; i < vc.size(); ++i)
    {
    	vc[i]->print();
    }
    areas = al->getConnectedAreas(vc);
    //for (int i = 0; i < areas.size(); ++i) {
    //	cout << "areas2Robobts::areas2Robobts " << areas[i].size() << endl;
    //}
}

subArea* areas2Robobts::lookForNewArea(myTuple location){
	vector<subArea*> a = sortedAvailableAreasPerLocation(location, NotAssigned);
	if(a.size()>0){
		return a[0];
	}
	return NULL;	
}

vector<subArea*> areas2Robobts::statrAllocation(vector<myTuple> teamStartLocations)
{	
	grid* g = grid::getInstance();
	for (int i = 0; i <teamStartLocations.size(); ++i)
	{
		int x = teamStartLocations[i].returnFirst();
		int y = teamStartLocations[i].returnSecond();
		pathCell* c = g->getCellAt(x,y);
		c->changeState();
	}
	//connect robots to areas
	vector<subArea*> sortedAreas;
	for(int i = 0;i < teamStartLocations.size();i++){	
		sortedAreas = sortedAvailableAreasPerLocation(teamStartLocations[i], NotAssigned);	
		for(int j = 0;j < sortedAreas.size();j++){
			subArea* a = sortedAreas[j];
			if(a->getinitialRobots().size()*D <= a->getCells().size()){//check if area is not too dense with robots
				a->addRobot(i);
				break;
			}
		}
	}

	//create the final assignment
	vector<subArea*> assignment;
	assignment.resize(teamStartLocations.size());
	
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
		}
	}
	return assignment;
}

vector<subArea*> areas2Robobts::sortedAvailableAreasPerLocation(myTuple location, AreaState askedState){
	vector<costedArea*> costedAreas;
	vector<subArea*> safests = getSafeAreas();
	//cout<<"safests"<<safests.size();
	for(int j = 0;j < safests.size();j++){
		subArea* area = safests[j];
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
	//cout<<"sortedAvailableAreasPerLocation"<<sortedAreas.size();
	return sortedAreas;
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

//add new robot to the area's list at it's level
void areas2Robobts::add(subArea* added){
 	int level = added->getLevel();
 	areas[level-1].push_back(added);//
 }



vector<subArea*> areas2Robobts::getSafeAreas(){
	int i = 0;
	if(areas.size() == 0){
		vector<subArea*> empty;
		return empty;
	}
	for (int a = 0; a < areas.size(); ++a) {
		vector<subArea*> unCoveredAtLevel;
		for (int b = 0; b < areas[a].size(); ++b) {
			if (areas[a][b]->getState() != Covered && areas[a][b]->getState() == NotAssigned) {
				unCoveredAtLevel.push_back(areas[a][b]);	
			}
		}
		areas[a] = unCoveredAtLevel;
	}

	while(areas[i].size() == 0){
		i++;
	}

	if (i == areas.size()) {
		vector<subArea*> empty;
		return empty;
	}

	cout << "areas2Robobts::getSafeAreas " << areas[i].size() << " " << i << endl;


	return areas[i];
}

 
 costedPath* areas2Robobts::findSafestPath(myTuple robiLocation, subArea* area){
 	vector<pathCell*> cells = area->getCells();
 	safestPath sp(cells);
 	return sp.find(robiLocation);    
 }


vector<pathCell*> areas2Robobts::getSafestPath(myTuple robiLocation, subArea* area){
	costedPath* cp = findSafestPath(robiLocation,area);
	return cp->getPath();
}
