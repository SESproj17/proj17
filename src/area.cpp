#include "area.h"

area::area(vector<vector <pathCell*> > givenCells,float givenProb,int lvl) {
	this->myCells = givenCells;
	this->prob = givenProb;
	this->myLevel = lvl;
	this->state = NotAssigned;
}
pathCell* area::getCellAt(int i,int j){
	return ((this->myCells[i])[j]);
}
float area::getProb() {return this->prob;}
int area::getLevel() {return this->myLevel;}
void area::changeState(AreaState newState) { this->state = newState; }
AreaState area::getState() {return this->state;}
bool area::isCovered() { return this->state == Covered;}

