#include "algo1.h"
#include <stdio.h>

using namespace std;

/*
input: jump- means how to jump between the probabilities
limit- the highest probability
*/
vector<area*> algo1::make_areas(float jump,float limit) {
	int i,j,rows,cols;
	float level;
	grid* myGrid = grid::getInstance();
	rows = myGrid->getRows();
	cols = myGrid->getCols();

	for(level = 0;level <= limit;level= level +jump){
		//init levelCells:
		vector<vector <pathCell*> > levelCells;
		levelCells.resize(rows);
		for (int k = 0; k < rows; ++k) {
			levelCells[k].resize(cols);
		}
		int levelInt = level*100;
		for(i = 0;i<rows;i++){
			for(j=0;j<cols;j++){
				pathCell* cur = myGrid->getCellAt(i,j);
				int x = cur->getProb()*100;
				if(x == levelInt){
					pathCell* areaCell = new pathCell(new myTuple(i,j),level,0,0);//id&cost!!!
					levelCells[i][j] = areaCell;
				}else{
					if(level == 0.3){
						cout<<cur->getProb() <<endl;
					}
					levelCells[i][j] = NULL;
				}
			}
		}
		//set the neighbours for each pathCell
		this->setNeighbours(levelCells);
		//create an area and push it to the list
		myAreas.push_back(new area(levelCells,level,level*10));
	}
	return myAreas;
}

void algo1::setNeighbours(vector<vector <pathCell*> > levelCells){
	int i,j,rows,cols;
	rows = levelCells.size();
	cols = levelCells[0].size();
	for(i = 0;i < rows;i++){
		for(j = 0;j < cols;j++){
			if(levelCells[i][j] != NULL){
				//up
				if(i-1 >= 0 && levelCells[i-1][j] != NULL){
					(levelCells[i][j])->addNeighbors(levelCells[i-1][j]);
				}
				//down
				if(i+1 < rows && levelCells[i+1][j] != NULL){
					(levelCells[i][j])->addNeighbors(levelCells[i+1][j]);
				}
				//right
				if(j+1 < cols && levelCells[i][j+1] != NULL){
					(levelCells[i][j])->addNeighbors(levelCells[i][j+1]);
				}
				//left
				if(j-1 >= 0 && levelCells[i][j-1] != NULL){
					(levelCells[i][j])->addNeighbors(levelCells[i][j-1]);
				}
			}
		}
	}
}

/*vector<subArea*> algo1::getConnectedAreas(vector<area*> areas){
	vector<subArea*> connected;//will hold all the connectedAreas in all the levels
	grid* myGrid = grid::getInstance();
	int rows = myGrid->getRows();
	int cols = myGrid->getCols();
	for(int i = 0;i < areas.size();i++){
		vector<subArea*> temp;
		temp = this->dfs(areas[i],rows,cols);
		for(int j = 0;j < temp.size();j++){
			connected.push_back(temp[j]);
		}
	}
	return connected;
}*/

vector<vector<subArea*> > algo1::getConnectedAreas(vector<area*> areas){
	vector<vector<subArea*> > connected;//will hold all the connectedAreas in all the levels
	connected.resize(areas.size());
	grid* myGrid = grid::getInstance();
	int rows = myGrid->getRows();
	int cols = myGrid->getCols();
	for(int i = 0;i < areas.size();i++){
		connected[i] = this->dfs(areas[i],rows,cols);
	}
	return connected;
}


//output : all the subareas from the input area
vector<subArea*> algo1::dfs(area* graph,int rows,int cols){
	int i,j;
	vector<subArea*> connectedList;
	vector<bool> visited(rows*cols, false);
	for(i = 0;i <rows;i++){
		for(j = 0;j<cols;j++){
			pathCell* s = graph->getCellAt(i,j);
			if(s != NULL && !visited[i*rows + j]){
				stack<pathCell*> stack;
				vector<vector<pathCell*> >subA(rows,vector<pathCell*>(cols, NULL));
				stack.push(s);
				while (!stack.empty())
				{ 
					s = stack.top();
					stack.pop();
					int x = s->getLocation().returnFirst(),y = s->getLocation().returnSecond();
					if (!visited[x*rows + y]){
						subA[x][y] = graph->getCellAt(x,y);
						visited[x*rows + y] = true;
					}
					//go over the neighbours if some of them are 
					//not visited push them into the stack
					vector<pathCell*>neib = s->getNeighbors();
					for (int i =0; i< neib.size();i++){
						int first,second;
						myTuple mt = neib[i]->getLocation();
						first = mt.returnFirst();
						second = mt.returnSecond();
						if(!visited[first*rows + second]){
							stack.push(neib[i]);
						}
					}
				}
				//create an area and add it to the vector
				subArea* connectedArea = new subArea(subA,graph->getProb(),graph->getLevel());
				connectedArea->changeState(NotAssigned);
				connectedList.push_back(connectedArea);
			}
		}
	}
	return connectedList;
}





