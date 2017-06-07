#ifndef TUPLE_H_TUP
#define TUPLE_H_TUP

#include <ros/ros.h>
#include <iostream>

class myTuple {
private:
	int x;
	int y;
public:
	myTuple(int givenX,int givenY);
	myTuple(); 
	int returnFirst();
	bool equals(myTuple* other);
	int returnSecond();
};


#endif