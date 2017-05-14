#include "costedPath.h"


float costedPath::getCost(){
	return this->myCost;
}
vector<pathCell*> costedPath::getPath(){
	return this->myCells;
}
costedPath::costedPath(vector<pathCell*> path, float price){
	this->myCost = price;
	this->myCells = path;
}