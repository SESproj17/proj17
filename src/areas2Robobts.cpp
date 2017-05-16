
#include "areas2Robobts.h"
using namespace std;

struct myclass {
  bool operator() ( costedArea* first,costedArea* second) 
  { return (first->getCost() < second->getCost());}
} compByCost;

areas2Robobts::areas2Robobts(vector<vector<subArea*> >givenConnectedAreas,vector<int>idsOfTeam){
	team = givenTeam;
	areas = givenConnectedAreas;
}

map<int, costedArea*> areas2Robobts::allocate(){
	
	vector<subArea*> safests = areas[0];
	vector<int>remove;
	vector<subArea*> add;

	//assign robots in areas
	for(int i = 0;i < team.size();i++){
		robot* robi = team[i];
		vector<costedArea*> costedAreas;
		for(int j = 0;j < safests.size();j++){
			costedAreas.push_back(new costedArea(safests[j], findSafestPath(robi->getLocation(),safests[j])));
		}
		sort (costedAreas.begin(), costedAreas.end(), compByCost);//by cost
		
		//try to put robi in area a:
		for(int j = 0;j < costedAreas.size();j++){//check if area is not too dense with robots
			subArea* a = costedAreas[j]->getArea();
			if(a->getinitialRobots().size()*D <= a->getCells().size()){
				a->addRobot(robi);
			}
		}
	}
	
	for(int i = 0;i <safests.size();i++){
		subArea* a = safests[i];
		if(a->getinitialRobots().size()>1){
			remove.push_back(i);			
			vector<subArea*>newAreas = splitAreaBetweenHisrobots(a);
			for (int j = 0; j < newAreas.size(); ++j)
			{
				add.push_back(newAreas[j]);
			}
		} else if (a->getinitialRobots().size()==1){
			robot* robi = a->getinitialRobots()[0];
			a->setWorker(robi);
			robi->setArea(a);
			vector<pathCell*> p = findSafestPath(robi->getLocation(), a)->getPath();
			robi->setPath(p);
		}

	}

	areas[0] = addAndRemove(safests, add, remove);
}


 costedPath* areas2Robobts::findSafestPath(myTuple* robiLocation, subArea* area){
 	grid* g = grid::getInstance();
 	vector<costedPath*>costedPaths;
 	vector<pathCell*> cells = area->getCells();
	for (int k = 0; k < cells.size(); ++k)
	{
		myTuple loc = cells[k]->getLocation();
		int goalI = loc.returnFirst();
		int goalJ = loc.returnSecond();
		vector<pathCell*> path = g->dijkstra(robiLocation->returnFirst(),robiLocation->returnSecond(),goalI,goalJ);
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


vector<subArea*> areas2Robobts::splitAreaBetweenHisrobots(subArea* area){
	int k = area->getinitialRobots().size();
	
	//split
	vector<vector<float> >costMatrix;
	costMatrix.resize(k);
		for (int l = 0; l < k; ++l) {
			costMatrix[l].resize(k);
		}
	vector<vector<vector<pathCell*> > >pathMatrix;
	pathMatrix.resize(k);
	for (int l = 0; l < k; ++l) {
		pathMatrix[l].resize(k);
	}

	vector<subArea*>newAreas;

	vector<vector<pathCell*> > splitedGraph = graphPartition(area->getCells(),k);
	for (int i = 0; i < k; ++i)
	{
		subArea* littleArea = new subArea(splitedGraph[i],area->getProb(),area->getLevel());
		newAreas.push_back(littleArea);
		for (int j = 0; j < k; ++j)
		{
			costedPath* cp = findSafestPath(area->getinitialRobots()[j]->getLocation(), littleArea);
			costMatrix[i][j] = cp->getCost();
			pathMatrix[i][j] = cp->getPath();
		}

	}

	//assign
	vector<myTuple*> assignments = hungarianMethod(costMatrix);

	for (int x = 0; x < assignments.size(); ++x)
	{
		int i = assignments[x]->returnFirst();
		int j = assignments[x]->returnSecond();
		robot* robi = area->getinitialRobots()[j];
		newAreas[i]->setWorker(robi);
		robi->setArea(newAreas[i]);
		robi->setPath(pathMatrix[i][j]);
	}
	

	return newAreas;
}

vector<subArea*> areas2Robobts::addAndRemove(vector<subArea*> safests, vector<subArea*>add, vector<int>remove){
	vector<subArea*> afterChange;
	for (int i = 0; i < remove.size(); ++i)
	{
		safests[remove[i]] = NULL;
	}
	for (int i = 0; i < safests.size(); ++i)
	{
		if(NULL!=safests[i]){
			afterChange.push_back(safests[i]);
		}
	}
	for (int i = 0; i < add.size(); ++i)
	{
		afterChange.push_back(add[i]);
	}
	return afterChange;
}


vector<vector<pathCell*> > areas2Robobts::graphPartition(vector<pathCell*>graph,int k){
	//algorithm
}

vector<myTuple*> areas2Robobts::hungarianMethod(vector<vector<float> >costMatrix){
	//algorithm
}






