#include "area.h"
#include "grid.h"
#include "algo1.h"

using namespace std;

/*
input: jump- means how to jump between the probabilities
limit- the highest probability
*/
vector<area*> make_areas(float jump,float limit){
	int i,j,rows,cols,level;
	grid* myGrid = grid.getInstance();
	rows = *myGrid.getRows();
	cols = *myGrid.getCols();
	for(level = 0;level <= limit;level+= jump){
		vector<vector <pathCell*>> levelCells;
		for(i = 0;i<rows;i++){
			for(j=0;j<cols;j++){
				pathCell* cur = *myGrid.getCellAt(i,j);
				if(*cur.getProb() == level){
					pathCell* areaCell = new pathCell();
					levelCells.pushback();
				}
			}
		}
		//create an area and push it to the list
		myAreas.pushback();
	}


}