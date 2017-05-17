#include "pathCell.h"
#include "cell.h"

pathCell::pathCell(myTuple* givenLocation,float givenProb,int givenId,float givenCost) : cell(givenLocation,givenProb,givenId) {
	ros::NodeHandle node;
	float minCost;
	if (givenCost == 0) { this->cost= 0;}
	else {
	 node.getParam("min_cost",minCost);
	 this->cost = givenCost/minCost;
	}
	this->lastCell = NULL;

}
//Set the cost of the cell
void pathCell::setCost(float newCost) {this->cost = newCost;}

//return the last cell of the path.
pathCell* pathCell::getLastCell() {return this->lastCell;}

//get the cost of the cell.
float pathCell::getCost() {return this->cost;}

//set the last path cell.
void pathCell::setLastPathCell(pathCell* lastOne) { this->lastCell = lastOne;}


void pathCell::addNeighbors(pathCell* neigh) {
	this->neighbors.push_back(neigh);
}

vector<pathCell*> pathCell::getNeighbors() { return this->neighbors;}

/*
void pathCell::setArea(subArea* givenArea){
	this->area = givenArea;
}*/