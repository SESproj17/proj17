#ifndef PATH_CELL
#define PATH_CELL

#include <ros/ros.h>
#include <iostream>
#include "cell.h"

//class subArea;
class pathCell: public cell {
private:
	float cost;
	pathCell* lastCell;
	vector<pathCell*> neighbors;
	//subArea* area;

public:
	pathCell(myTuple* givenLocation,float givenProb,int givenId,float givenCost);
	void setCost(float newCost);

	void addNeighbors(pathCell* neigh);
	
	pathCell* getLastCell();
	float getCost();
	void setLastPathCell(pathCell* lastOne);
	vector<pathCell*> getNeighbors();
	//void setArea(subArea* givenArea);
};
#endif
