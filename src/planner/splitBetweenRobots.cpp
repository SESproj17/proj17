#include "splitBetweenRobots.h"
#include <stack> 


//the vector of the robots is in the according order of the locations order of the robots
splitBetweenRobots::splitBetweenRobots(subArea* areaToSplit, vector<myTuple> locations){
	//cout<<"splitBetweenRobots:constructor"<<endl;
	initialArea = areaToSplit;
	//initialArea->print();
	robotLocations = locations;
	//cout<<"splitBetweenRobots:robotLocations size "<<robotLocations.size()<<endl;
}

splitBetweenRobots::splitBetweenRobots(){
	
}

vector<vector<pathCell*> > splitBetweenRobots::privateSplit(int k){	
	//vector<vector<subArea*> > preSpliting = initialArea->getInheritance();
	//if(preSpliting.size()> 1){
	//	cout<<"splitBetweenRobots:: unConnected left area"<<

	//}
	vector<pathCell*> path = initialArea->coverge(robotLocations[0]);
	//cout<<"path size: "<<path.size()<<endl;
	int amount = path.size() / k;
	//remainder = cells % k;
	vector<vector<pathCell*> > splitedPath;
	splitedPath.resize(k);
	
	for (int i = 0; i < k-1; ++i)
	{
		splitedPath[i].insert(splitedPath[i].end(), path.begin()+i*amount, path.begin()+(i+1)*amount);//-1?
	}
	splitedPath[k-1].insert(splitedPath[k-1].end(),path.begin()+(k-1)*amount,path.end());

/*
	//check:
	for (int i = 0; i < splitedPath.size(); ++i)
	{
		cout<<"I:"<<i<<endl;
		vector<pathCell*> subpath = splitedPath[i];
		cout<<subpath.size()<<endl;
		for (int j = 0; j < subpath.size(); ++j)
		{	
			cout << subpath[j]->getLocation().returnFirst();
			cout<<";";
			cout << subpath[j]->getLocation().returnSecond();
			if (i!= subpath.size() -1) {
				cout<< " ";
			}
		}
		cout<<endl;
	}

	exit(0);
	*/
	
	return splitedPath;

}

//returns all the subareas of one sub area that was splited
vector<subArea*> splitBetweenRobots::split(){
	cout<<"split:Warning- only for 2 now!!"<<endl;
	vector<vector<pathCell*> > splitedPath = privateSplit(2);
	vector<subArea*>newAreas;
	newAreas.resize(2);
	grid* g = grid::getInstance();
	newAreas[0] = new subArea(g->getRows(),g->getCols(),splitedPath[0],initialArea->getProb(),initialArea->getLevel());	
	newAreas[1] = new subArea(g->getRows(),g->getCols(),splitedPath[1],initialArea->getProb(),initialArea->getLevel());	
	return newAreas;
}




//helps to find the location of a cell in vector according to his
// location
int splitBetweenRobots::findLocation(pathCell* cell, vector<pathCell*>graph){
	int first = cell->getLocation().returnFirst();
	int second = cell->getLocation().returnSecond();
	int size = graph.size();
	for(int i = 0; i < size; i++){
		pathCell* current = graph[i];
		int cur_f = current->getLocation().returnFirst();
		int cur_s = current->getLocation().returnSecond();
		if(first == cur_f && second == cur_s){
			return i;
		}
	}
	return -1;
}


//split the area and generates the costmatrix and find the optimal assignment
//for the robots
vector<subArea* > splitBetweenRobots::hungarianMethod(){
	cout<<"hungarianMethod:entered "<<endl;
	int k = initialArea->getinitialRobots().size();

	//init
	vector<vector<float> >costMatrix;
	costMatrix.resize(k);
		for (int l = 0; l < k; ++l) {
			costMatrix[l].resize(k);
		}
	
	vector<subArea*>newAreas;

	//split
	vector<vector<pathCell*> > splitedGraph = privateSplit(k);
	cout<<"hungarianMethod:after graphPartition! "<<endl;

	for (int i = 0; i < k; ++i)
	{
		cout<<"hungarianMethod:before sa constructor "<<endl;
		grid* g = grid::getInstance();
		subArea* littleArea = new subArea(g->getRows(),g->getCols(),splitedGraph[i],initialArea->getProb(),initialArea->getLevel());	
		//littleArea->print();
		cout<<"hungarianMethod:after sa constructor "<<endl;

		newAreas.push_back(littleArea);
		for (int j = 0; j < k; ++j)
		{	//robots are in columns
			safestPath sp(littleArea->getCells());
			costedPath* cp = sp.find(robotLocations[j]);
			costMatrix[i][j] = cp->getCost();
		}
	}

	//convert to ints
	int rows,cols;
	rows = costMatrix.size();
	cols = costMatrix[0].size();
	vector<vector<int> >intMatrix(rows,vector<int>(cols, 0));//initiallize
	for(int i = 0;i < rows;i++){
		for(int j = 0;j < cols;j++){
			intMatrix[i][j] = (int)costMatrix[i][j] * 100;
		}
	}

	cout<<"hungarianMethod:before hungarian() "<<endl;

	Hungarian* hung = new Hungarian(intMatrix,rows,cols,HUNGARIAN_MODE_MINIMIZE_COST);
	hung->solve();
	
	vector<subArea*> assignment = convert(hung->assignment(), newAreas);
	cout<<"hungarianMethod:assignment size "<<assignment.size()<<endl;
	return assignment;
}

//covert given output from the hungarian method to an assignment
vector<subArea*> splitBetweenRobots::convert(vector<vector<int> > HungarianOutput, vector<subArea*> areas){
	vector<subArea*> assignment(HungarianOutput.size(),NULL);
	for (int i = 0; i < HungarianOutput.size(); ++i)
	{
		for (int j = 0; j < HungarianOutput[0].size(); ++j)
		{
			if(HungarianOutput[i][j] == 1){
				//according to the assumption that robots in columns and jobs in rows
				assignment[j] = areas[i];
			}
		}
	}
	cout<<"convert::hungarian is done!"<<endl;
	return assignment;
}









