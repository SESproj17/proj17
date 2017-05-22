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
    this->notFoundYet = 0;
    this->notFoundYet = this->cells.size();
}


void subArea::print() {
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

int subArea::getLeftCells() { return this->notFoundYet; }

vector<pathCell*> subArea::coverge(myTuple start){
    print();
    grid* g = grid::getInstance();
    vector<pathCell*> path;
    vector<pathCell*> notAppearsAtPath;
    for (int i = 0; i < cells.size(); ++i)
    {
        if (cells[i]->getState()== NotVisited){
            notAppearsAtPath.push_back(cells[i]);
        }
    }
    while(notAppearsAtPath.size()){
        myTuple dest = notAppearsAtPath[0]->getLocation();
       vector<pathCell*> temp = g->dijkstra(start.returnFirst(),start.returnSecond(),dest.returnFirst(),dest.returnSecond());
        path.insert(path.end(), temp.begin(), temp.end());
        notAppearsAtPath.erase(notAppearsAtPath.begin());
        for (int i = 0; i < temp.size(); ++i)
        {
            temp[i]->setAppear();
        }
        temp.clear();
        for (int i = 0; i < notAppearsAtPath.size(); ++i)
        {
            if(!(notAppearsAtPath[i]->wasInPath())){
                temp.push_back(notAppearsAtPath[i]);
            }
        }
        notAppearsAtPath = temp; 
        start = path[path.size()-1]->getLocation();  
    }
    return path;
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
float subArea::getProb() {return this->prob;}
int subArea::getLevel() {return this->myLevel;}

void subArea::changeState(AreaState newState) {
     this->state = newState; 
 }
AreaState subArea::getState() {
    return this->state;
}

void subArea::notifyVisitedCell(){
    this->notFoundYet--;
    if (this->notFoundYet == 0){this->state = Covered;}
}

