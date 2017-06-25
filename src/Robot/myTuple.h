#ifndef TUPLE_H_TUP
#define TUPLE_H_TUP

#include <ros/ros.h>
#include <iostream>

class myTuple {
private:
	float x;
	float y;
public:
	myTuple(float givenX,float givenY);
	float returnFirst();
	bool equals(myTuple* other);
	float returnSecond();
};


#endif