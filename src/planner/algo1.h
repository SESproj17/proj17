#ifndef ALGO1_H
#define ALGO1_H


#include <ros/ros.h>
#include <iostream>
#include "myTuple.h"
#include "subArea.h"
#include <stack>
#include "area.h"
#include "grid.h"


class algo1{
private:
	vector<area*> myAreas;
	void setNeighbours(vector<vector <pathCell*> > levelCells);
	vector<subArea*> dfs(area* graph,int rows,int cols);
public:
	vector<area*> make_areas(float jump,float limit);
	//vector<subArea*> getConnectedAreas(vector<area*> areas);
	vector<vector<subArea*> > getConnectedAreas(vector<area*> areas);
};

#endif