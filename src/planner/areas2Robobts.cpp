
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
    areas = al->getConnectedAreas(vc);
}

subArea* areas2Robobts::lookForNewArea(myTuple location){
	//cout<<"lookForNewArea:start"<<endl;
	vector<subArea*> a = sortedAvailableAreasPerLocation(location, NotAssigned);
	//cout<<"lookForNewArea:nuber of safest unassiged areas: "<<a.size()<<endl;
	if(a.size()>0){
		return a[0];
	}
	//cout<<"lookForNewArea:no more areas to cover"<<endl;
	return NULL;	
}

vector<subArea*> areas2Robobts::statrAllocation(vector<myTuple> teamStartLocations)
{	
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
				//assignment[idsRobotsOfA[i]]->print();
			}
		} else if (idsRobotsOfA.size()==1){
			int id = a->getinitialRobots()[0];
			assignment[id] = a;
			cout<<"statrAllocation::area "<<endl;
			a->print();
			cout<<" assigned to robot "<<id<<endl;
		}
	}


	//debug code
	vector<subArea*> empty;
	areas[0] = empty;
	vector<subArea*> empty1;
	areas[1] = empty1;
	vector<subArea*> empty2;
	areas[2] = empty2;
	vector<subArea*> empty3;
	areas[3] = empty3;
	//debug code
	return assignment;
}

vector<subArea*> areas2Robobts::sortedAvailableAreasPerLocation(myTuple location, AreaState askedState){
	vector<costedArea*> costedAreas;
	vector<subArea*> safests = getSafeAreas();
	//cout<<"sortedAvailableAreasPerLocation:safests.size ; "<<safests.size()<<endl;
	for(int j = 0;j < safests.size();j++){
		subArea* area = safests[j];
		if(area->getState() == askedState){
			costedAreas.push_back(new costedArea(area, findSafestPath(location,area)));
		}
	}
	sort (costedAreas.begin(), costedAreas.end(), compByCost);//by cost
	//cout<<"after sort"<<endl;
	vector<subArea*> sortedAreas;
	for (int i = 0; i < costedAreas.size(); ++i)
	{
		sortedAreas.push_back(costedAreas[i]->getArea());
	}
	return sortedAreas;
}


/*
input: area(oldArea) that splited, new sub areas
the function marked the oldArea as covered and add the new areas to the pool
*/
void areas2Robobts::addSplited(subArea* oldArea, vector<subArea*> newSplitedAreas){
	//cout<<"addSplited:new areas "<<newSplitedAreas.size()<<endl;
	//cout<<"addSplited:areas before"<<areas[0].size()<<endl;
	oldArea->changeState(Covered);
	for (int i = 0; i < newSplitedAreas.size(); ++i)
	{
		add(newSplitedAreas[i]);
	}
	//cout<<"addSplited:areas after"<<areas[0].size()<<endl;

}
//add new robot to the area's list at it's level
void areas2Robobts::add(subArea* added){
 	int level = added->getLevel();
 	areas[level-1].push_back(added);//
 }



vector<subArea*> areas2Robobts::getSafeAreas(){
	int i = 0;
	while(areas[i].size() == 0){
		//cout<<"getSafeAreas:areas["<<i<<"].size() "<<areas[i].size()<<endl;
		i++;
		if(i == areas.size()){
			vector<subArea*> empty;
			return empty;
		}
	}
	vector<subArea*> level = areas[i];
	vector<subArea*> unCoveredAtLevel;
	//cout<<"getSafeAreas:level.size() "<<level.size()<<endl;
	for (int j = 0; j < level.size(); ++j)
	{
		if (level[j]->getState() != Covered)
		{
			unCoveredAtLevel.push_back(level[j]);
		}
	}
	areas[i] = unCoveredAtLevel;
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
