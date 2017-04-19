#ifndef ALGO1_H
#define ALGO1_H


#include <ros/ros.h>
#include <iostream>

class algo1{
private:
	vector<area*> myAreas;
public:
	vector<area*> make_areas(float jump,float limit);

}

#endif