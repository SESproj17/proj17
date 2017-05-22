
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
	cout<<"list of areas to robot: "<<a.size()<<endl;
	if(a.size()>0){
		return a[0];
	}
	subArea* area = findAreaToShare();
	return area;	
}

vector<subArea*> areas2Robobts::statrAllocation(vector<myTuple> teamStartLocations)
{	
	//assign robots in areas
	vector<subArea*> sortedAreas;
	for(int i = 0;i < teamStartLocations.size();i++){	
		sortedAreas = sortedAvailableAreasPerLocation(teamStartLocations[i], NotAssigned);
		cout<<"statrAllocation::size of sorted: "<<sortedAreas.size()<<endl;			
		for(int j = 0;j < sortedAreas.size();j++){//check if area is not too dense with robots
			subArea* a = sortedAreas[j];
			if(a->getinitialRobots().size()*D <= a->getCells().size()){// after 
				a->addRobot(i);
				cout<<"statrAllocation::a assigned "<<a->getLevel()<<endl;
				break;
			}
			cout<<"statrAllocation::need to not beeing here "<<endl;
		}
	}

	vector<subArea*> assignment;
	assignment.resize(teamStartLocations.size());
	
	//we can stop this loop when no robots left
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
			cout<<"statrAllocation::a->level: "<<assignment[id]->getLevel()<<endl;
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



subArea* areas2Robobts::findAreaToShare(){

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
	return price;
 }

 void areas2Robobts::add(subArea* added){
 	int level = added->getLevel();
 	areas[level].push_back(added);
 }
