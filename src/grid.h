#ifndef GRID_H
#define GRID_H


#include <ros/ros.h>
#include <iostream>
#include <vector>
#include "pathCell.h"


class grid {
private:
	static grid *instance;
	static bool isOn;

	pathCell* getMinCost(vector<pathCell*> thisCells);
	vector<pathCell*> removeCell(vector<pathCell*> thisCells,pathCell* current);

	vector<vector <pathCell*> > cells;
	int rows;
	int cols;
	vector<vector<float> > myGrid;
	
	void reduceMap();
	void initGrid();
	void readMap();
	grid();
public:
	static grid* getInstance();
	pathCell* getCellAt(int i,int j);
	void print();
	vector<pathCell*> dijkstra(int initI,int initJ,int goalI,int goalJ);

};

#endif