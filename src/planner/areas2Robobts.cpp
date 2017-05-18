
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

}

subArea* areas2Robobts::lookForNewArea(myTuple location){

}

vector<subArea*> areas2Robobts::statrAllocation(vector<myTuple> teamStartLocations)
{	
	cout<<"statrAllocation::number of robots: "<<teamStartLocations.size()<<endl;
	cout<<"statrAllocation::number of safe areas: "<<areas[0].size()<<endl;

	//assign robots in areas
	vector<subArea*> sortedAreas;
	for(int i = 0;i < teamStartLocations.size();i++){	
		sortedAreas = sortedAvailableAreasPerLocation(teamStartLocations[i], NotAssigned);				
		for(int j = 0;j < sortedAreas.size();j++){//check if area is not too dense with robots
			subArea* a = sortedAreas[j];
			if(a->getinitialRobots().size()*D <= a->getCells().size()){// after 
				a->addRobot(i);
				break;
			}
		}
	}

	vector<subArea*> assignment;
	assignment.resize(teamStartLocations.size());
	
	for(int i = 0;i <sortedAreas.size();i++){
		subArea* a = sortedAreas[i];
		vector<int> idsRobotsOfA= a->getinitialRobots();
		if(idsRobotsOfA.size()>1){	
			vector<subArea*> splited = split(idsRobotsOfA,a);
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
	//vector<subArea*> safests = getSafeAreas();
	vector<subArea*> safests = areas[0];
	for(int j = 0;j < safests.size();j++){
		subArea* area = safests[j];
		if(area->getState() == askedState){
			costedAreas.push_back(new costedArea(area, findSafestPath(location,safests[j])));
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


vector<subArea*> areas2Robobts::split(vector<int> robots, subArea* areaToSplit){
	areaToSplit->changeState(Covered);
	splitBetweenRobots sbr(robots, areaToSplit);
	vector<subArea*> splited = sbr.split();
	for (int i = 0; i < splited.size(); ++i)
	{
		add(splited[i]);
	}
	return splited;
}



vector<pathCell*> findAreaToShare(){

} 


 costedPath* areas2Robobts::findSafestPath(myTuple robiLocation, subArea* area){
 	grid* g = grid::getInstance();
 	vector<costedPath*>costedPaths;
 	vector<pathCell*> cells = area->getCells();
	for (int k = 0; k < cells.size(); ++k)
	{
		myTuple loc = cells[k]->getLocation();
		int goalI = loc.returnFirst();
		int goalJ = loc.returnSecond();
		vector<pathCell*> path = g->dijkstra(robiLocation.returnFirst(),robiLocation.returnSecond(),goalI,goalJ);
		costedPath* cp = new costedPath(path, price(path)); 
		costedPaths.push_back(cp);
	}
	return this->min(costedPaths);	        
 }

 costedPath* areas2Robobts::min(vector<costedPath*>paths){
 	costedPath* cp = paths[0];
 	float minimum = cp->getCost();
 	for (int i = 0; i < paths.size(); ++i)
 	{
 		if(paths[i]->getCost()< minimum){
 			minimum = paths[i]->getCost();
 			cp = paths[i];
 		}
 	}
 	return cp;

 }

 float areas2Robobts:: price(vector<pathCell*> path){
 	float price = 0.0;
 	for (int k = 0; k < path.size(); ++k)
	{
		price += path[k]->getCost();
	}
 }

 void areas2Robobts::add(subArea* added){
 	int level = added->getLevel();
 	areas[level].push_back(added);
 }
