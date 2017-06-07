#include "myTuple.h"


myTuple::myTuple(int givenX,int givenY) {
	this->x = givenX;
	this->y = givenY;
}

myTuple::myTuple() {
	this->x = -1;
	this->y = -1;
}

//return the first elemnt.
int myTuple::returnFirst() { return this->x;}
//return the second element.
int myTuple::returnSecond() {return this->y;}
bool myTuple::equals(myTuple* other) {
	return this->x == other->returnFirst() && this->y == other->returnSecond();
}