#include "myTuple.h"


myTuple::myTuple(int givenX,int givenY) {
	this->x = givenX;
	this->y = givenY;
}

//return the first elemnt.
int myTuple::returnFirst() { return this->x;}
//return the second element.
int myTuple::returnSecond() {return this->y;}

