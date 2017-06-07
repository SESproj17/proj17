#ifndef COSTEDAREA_H
#define COSTEDAREA_H


#include <ros/ros.h>
#include "subArea.h"
#include "costedPath.h"


//A connction between area to path and price to it 
class costedArea{
	private:
		subArea* myArea;
		costedPath* cp;
	public:
		costedArea(subArea* myArea, costedPath* cp);
		float getCost();
		subArea* getArea();
		vector<pathCell*> getPath();
};

#endif

