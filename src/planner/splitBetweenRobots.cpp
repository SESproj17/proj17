#include "splitBetweenRobots.h"



//the vector of the robots is in the according order of the locations order of the robots
splitBetweenRobots::splitBetweenRobots(subArea* areaToSplit, vector<myTuple> locations){
	cout<<"splitBetweenRobots:constructor"<<endl;
	initialArea = areaToSplit;
	initialArea->print();
	robotLocations = locations;
	cout<<"splitBetweenRobots:robotLocations size "<<robotLocations.size()<<endl;
}

splitBetweenRobots::splitBetweenRobots(){
	
}

//returns all the subareas of one sub area that was splited
vector<subArea*> splitBetweenRobots::split(){
	int k = initialArea->getinitialRobots().size();
	vector<subArea*>newAreas;
	vector<vector<pathCell*> > splitedGraph = graphPartition(initialArea->getCells(),k);

	for (int i = 0; i < k; ++i)
	{
		subArea* littleArea = new subArea(vectorToMatrix(splitedGraph[i]),initialArea->getProb(),initialArea->getLevel());
		newAreas.push_back(littleArea);
	}
	return newAreas;
}

//create matrices of pathCells* for input to create subArea
vector<vector<pathCell*> > splitBetweenRobots::vectorToMatrix(vector<pathCell*> vec){
	int rows, cols;
	grid* grd = grid::getInstance();
	rows = grd->getRows();
	cols = grd->getCols();
	vector<vector<pathCell*> > matrix(rows,vector<pathCell*>(cols, NULL));
	for(int i = 0; i < vec.size();i++){
		myTuple tup = vec[i]->getLocation();
		int f = tup.returnFirst();
		int s = tup.returnSecond();
		matrix[f][s] = vec[i];
	}
	return matrix;
}

vector<vector<pathCell*> > splitBetweenRobots::graphPartition(vector<pathCell*>graph,int k){

	int remainder = 0;
	int cells = graph.size();
	cout<<"graphPartition:cells "<<cells<<endl;
	vector<bool> visited(cells, false);
	vector<pathCell*> last;
	//how many cells in each vector
	int amount = cells / k;
	vector<vector<pathCell*> > solution(k-1, vector<pathCell*>(amount,NULL));
	//cout<<"graphPartition:solution size "<<solution.size()<<endl;
	//if the number of cells does not divided in k
	if(cells % k != 0){
		remainder = cells % k;
		last.resize(amount + remainder, NULL);
	}else{
		last.resize(amount, NULL);
	}
	solution.push_back(last);
	cout<<"graphPartition:solution[0] size "<<solution[0].size()<<endl;
	cout<<"graphPartition:solution[1] size "<<solution[1].size()<<endl;
	//the number of the cell that we are adding him and his neibours to the solution
	for(int i = 0; i < k;i++){
		for(int q = 0; q < cells; q++){
			for(int j = 0;j < amount;){
				if(visited[q] == false){			
					solution[i][j] = graph[q];//add yourself
					visited[q] = true;
					j++;
					vector<pathCell*> neib = graph[q]->getNeighbors();
					cout<<"graphPartition:neib size "<<neib.size()<<endl;
					for(int r = 0; r < neib.size();r++){
						pathCell* c = neib[r];
						cout<<"graphPartition:c as n "<<c->getProb()<<endl;
						if(graph[q]->getProb()==c->getProb()){
							int index = findLocation(c, graph);
							if(visited[index] == false){
								visited[index] = true;
								solution[i][j] = c;
								j++;
							}
						}
					}
				}else{
					q++;}//move to the next cell in graph
			}
		}
		cout<<"graphPartition:0th-0th "<<solution[0][0]->getProb()<<endl;
	}
	cout<<"graphPartition:here "<<endl;

	//the last vector -- add the remain cells
	for(int i = 0; i < cells; i++){
		if(visited[i] == false){
			solution[k-1].push_back(graph[i]); 
		}
	}

	return solution;
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

	//split
	vector<vector<float> >costMatrix;
	costMatrix.resize(k);
		for (int l = 0; l < k; ++l) {
			costMatrix[l].resize(k);
		}
	


	vector<subArea*>newAreas;

	vector<vector<pathCell*> > splitedGraph = graphPartition(initialArea->getCells(),k);
	cout<<"hungarianMethod:after graphPartition! "<<endl;
	for (int i = 0; i < k; ++i)
	{
		subArea* littleArea = new subArea(vectorToMatrix(splitedGraph[i]),initialArea->getProb(),initialArea->getLevel());
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

	Hungarian* hung = new Hungarian(intMatrix,rows,cols,HUNGARIAN_MODE_MINIMIZE_COST);
	hung->solve();
	//return hung->assignment();
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









