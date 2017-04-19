#include "subArea.h"

subArea::subArea(vector<vector <int> > givenCells,float givenProb,int lvl) {
	this->myCells = givenCells;
	this->prob = givenProb;
	this->myLevel = lvl;
	this->state = NotAssigned;
}

float subArea::getProb() {return this->prob;}
int subArea::getLevel() {return this->myLevel;}
void subArea::changeState(AreaState newState) { this->state = newState; }
AreaState subArea::getState() {return this->state;}