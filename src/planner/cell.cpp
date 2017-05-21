#include "cell.h"
#include "subArea.h"
#include <stdlib.h>

cell::cell(myTuple* givenLocation,double givenProb,int givenId) {
	this->location = givenLocation;
	this->prob = givenProb;
	this->myState = NotVisited;
	this->id = givenId;
}


void cell::setArea(subArea* givenArea){
	////debug code
	//cout<< "cell::setArea: givenArea level"<< givenArea->getLevel()<<endl;
	////debug code
	this->area = givenArea;
	////debug code
	//cout<< "cell::setArea: localarea level"<< this->area->getLevel()<<endl;
	////debug code
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

//if some robot walk on this cell mark it.
void cell::changeState() {
	this->myState = Visited;
	//debug code
	cout<< "cell::changeState: before notifyVisitedCell"<< endl;
	cout<< "cell::changeState: is area exist??: "<< this->area->getLevel() << "so it is.."<<endl;
	//debug code

	this->area->notifyVisitedCell();

	//debug code
	cout<< "cell::changeState: after notifyVisitedCell"<< endl;
	//debug code
}

//flip coin and check if the robot that move on this cell died or not.
bool cell::imAlive() {
	srand(time(NULL));
	int x;float y;
	x = rand()%11;
	y = float(x)/10;
	if (y > this->prob) {return true;}
	return false;
}
void cell::setProb(float p) {
	this->prob = p;
}