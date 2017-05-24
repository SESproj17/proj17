
#ifndef SP_H
#define SP_H

#include <ros/ros.h>
#include <vector>
#include "pathCell.h"
#include "costedPath.h"

using namespace std;

class safestPath{
private:

	vector<pathCell*> cells;

	costedPath* min(vector<costedPath*>paths);
	float price(vector<pathCell*> path);

	
public:
	safestPath(vector<pathCell*> cells);
	costedPath* find(myTuple robiLocation); 
	
};

#endif

