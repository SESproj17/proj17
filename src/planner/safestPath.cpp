#include "safestPath.h" 


safestPath::safestPath(vector<pathCell*> givencells){
	cells = givencells;
}

costedPath* safestPath::find(myTuple robiLocation){
	grid* g = grid::getInstance();
 	vector<costedPath*>costedPaths;
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

costedPath* safestPath::min(vector<costedPath*>paths){
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

 float safestPath::price(vector<pathCell*> path){
 	float price = 0.0;
 	for (int k = 0; k < path.size(); ++k)
	{
		price += path[k]->getCost();
	}
	return price;
 }