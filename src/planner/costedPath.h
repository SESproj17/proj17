#ifndef COSTEDPATH_H
#define COSTEDPATH_H

#include <ros/ros.h>
#include <iostream>
#include <vector>
#include "grid.h"

//A connection between path and its price
class costedPath {
private:
	vector<pathCell*> myCells;
	float myCost;
public:
	costedPath(vector<pathCell*> path, float price);
	float getCost();
	vector<pathCell*> getPath();
};


#endif