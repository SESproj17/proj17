#include "costedArea.h"

costedArea::costedArea(subArea* myArea, costedPath* cp){
	this->myArea = myArea;
	this->cp = cp;
}

float costedArea::getCost(){
	return this->cp->getCost();
}

subArea* costedArea::getArea(){
	return this->myArea;
}

costedPath* costedArea::getMyPath(){
	return this->cp;
}