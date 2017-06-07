#include "cell.h"
#include "subArea.h"
#include <stdlib.h>

cell::cell(myTuple* givenLocation,double givenProb,int givenId) {
	this->location = givenLocation;
	this->prob = givenProb;
	this->myState = NotVisited;
	this->id = givenId;
}

//A cell knows about the area that holds it
void cell::setArea(subArea* givenArea){	
	this->myArea = givenArea;
}
//return the probability of the cell
float cell::getProb() {return this->prob;}

//return the (x,y) on the grid of this cell.
myTuple cell::getLocation() {return (*this->location);}

//return the id of this cell.
int cell::getId() {return this->id;}

//get another cell and check if they are equal.
bool cell::isEqual(cell* other) { 
	return other->getId() == this->id;
}

//return the state of the cell.
State cell::getState() {return this->myState;}

//if some robot walk on this cell mark it. update the holding aera about that
void cell::changeState() {	
	this->myState = Visited;
	this->myArea->notifyVisitedCell();

}


void cell::setProb(float p) {
	this->prob = p;
}

void cell::setLevel(int level){
	this->level = level;
}
int cell::getLevel(){
	return this->level;
}