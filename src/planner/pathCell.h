#ifndef PATH_CELL
#define PATH_CELL

#include <ros/ros.h>
#include <iostream>
#include "cell.h"


class pathCell: public cell {
private:
	float cost;
	pathCell* lastCell;
	vector<pathCell*> neighbors;
	bool wasInCoveragePath;
	

public:
	pathCell(myTuple* givenLocation,float givenProb,int givenId,float givenCost);
	void setCost(float newCost);

	void addNeighbors(pathCell* neigh);
	bool wasInPath();
	void setAppear(bool v);
	pathCell* getLastCell();
	float getCost();
	void setLastPathCell(pathCell* lastOne);
	vector<pathCell*> getNeighbors();
	
};
#endif
