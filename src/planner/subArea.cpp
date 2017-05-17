#include "subArea.h"


subArea::subArea(vector<vector <pathCell*> > givenCells,float givenProb,int lvl) {
	this->myCells = givenCells;
	this->prob = givenProb;
	this->myLevel = lvl;
	this->state = NotAssigned;
    for (int i = 0; i < givenCells.size(); ++i)
    {
        for (int j = 0; j < givenCells[i].size(); ++j)
        {
            if(givenCells[i][j]){
                this->cells.push_back(givenCells[i][j]);
            }
        }
    }
}

subArea::subArea(vector<pathCell* > newCells,float givenProb,int lvl){
    this->prob = givenProb;
    this->myLevel = lvl;
    this->state = NotAssigned;
    this->cells = newCells;

}

vector<pathCell*> subArea::getCells(){
    return this->cells;
}

pathCell* subArea::getCellAt(int i,int j){
	return ((this->myCells[i])[j]);
}
vector<int> subArea::getinitialRobots(){return this->initialRobots;}
void subArea::addRobot(int robi){
    this->initialRobots.push_back(robi);
}
/*
void subArea::setWorker(robot* robi){
    this->worker = robi;
    
}*/
float subArea::getProb() {return this->prob;}
int subArea::getLevel() {return this->myLevel;}
void subArea::changeState(AreaState newState) { this->state = newState; }
AreaState subArea::getState() {return this->state;}
