#include "myTuple.h"


myTuple::myTuple(float givenX,float givenY) {
	this->x = givenX;
	this->y = givenY;
}
//return the first elemnt.
float myTuple::returnFirst() { return this->x;}
//return the second element.
float myTuple::returnSecond() {return this->y;}
bool myTuple::equals(myTuple* other) {
	return this->x == other->returnFirst() && this->y == other->returnSecond();
}