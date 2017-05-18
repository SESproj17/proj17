#include "splitBetweenRobots.h"

/*
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
}*/
splitBetweenRobots::splitBetweenRobots(vector<int> robots, subArea* areaToSplit){
	
}

vector<subArea*> splitBetweenRobots::split(){

}

vector<vector<pathCell*> > splitBetweenRobots::graphPartition(vector<pathCell*>graph,int k){
	//algorithm
}

vector<myTuple*> splitBetweenRobots::hungarianMethod(vector<vector<float> >costMatrix){
	//algorithm
}