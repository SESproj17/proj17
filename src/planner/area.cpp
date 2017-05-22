#include "area.h"

area::area(vector<vector <pathCell*> > givenCells,float givenProb,int lvl) {
	this->myCells = givenCells;
	this->prob = givenProb;
	this->myLevel = lvl;
	this->state = NotAssigned;
}
pathCell* area::getCellAt(int i,int j){
	return ((this->myCells[i])[j]);
}
float area::getProb() {return this->prob;}
int area::getLevel() {return this->myLevel;}
void area::changeState(AreaState newState) { this->state = newState; }
AreaState area::getState() {return this->state;}
bool area::isCovered() { return this->state == Covered;}

void area::print() {
	cout<<"level: "<<myLevel<<endl;
    for (int i = 0; i < myCells.size(); ++i)
    {
        for (int j = 0; j < myCells[i].size(); ++j)
        {
            if(myCells[i][j] == NULL){
                cout << "O ";
            }else {
                cout << "1 ";
            }
        }
        cout << endl;
    }    
}

