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


//output : all the subareas of this area
vector<subArea*> subArea::dfs(){
    grid* g = grid::getInstance();
    int rows = g->getRows();
    int cols = g->getCols();
    int i,j;
    vector<subArea*> connectedList;
    vector<bool> visited(rows*cols, false);
    for(i = 0;i <rows;i++){
        for(j = 0;j<cols;j++){
            pathCell* s = g->getCellAt(i,j);
            if(s != NULL && s->getState() == NotVisited && !visited[i*rows + j]){
                stack<pathCell*> stack;
                vector<vector<pathCell*> > subA(rows,vector<pathCell*>(cols, NULL));
                stack.push(s);
                while (!stack.empty())
                { 
                    cout<<i<<" "<<j<<endl;
                    s = stack.top();
                    stack.pop();
                    int x = s->getLocation().returnFirst(),y = s->getLocation().returnSecond();
                    if (!visited[x*rows + y]){
                        subA[x][y] = g->getCellAt(x,y);
                        visited[x*rows + y] = true;
                    }
                    //go over the neighbours if some of them are 
                    //not visited push them into the stack
                    vector<pathCell*>neib = s->getNeighbors();
                    //cout<<"sizeOfneibs: "<<neib.size()<<endl;
                    for (int i =0; i< neib.size();i++){
                        int first,second;
                        myTuple mt = neib[i]->getLocation();
                        first = mt.returnFirst();
                        second = mt.returnSecond();
                        if(!visited[first*rows + second] && neib[i]->getState() == NotVisited){
                            stack.push(neib[i]);
                        }
                    }
                    //cout<<"sizeOstack: "<<stack.size()<<endl;
                    //exit(0);
                }
                //create an area and add it to the vector
                subArea* connectedArea = new subArea(subA,prob,myLevel);
                connectedArea->changeState(NotAssigned);

                vector<pathCell*> cells = connectedArea->getCells();
                for (int k = 0; k < cells.size(); ++k)
                {
                    int xRows = cells[k]->getLocation().returnFirst();
                    int yCols = cells[k]->getLocation().returnSecond();
                    pathCell* myCell = g->getCellAt(xRows,yCols);
                
                    myCell->setArea(connectedArea);
                }
                connectedList.push_back(connectedArea);
            }
        }
    }
    return connectedList;
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
            cells[i]->setAppear(false);
            notAppearsAtPath.push_back(cells[i]);
        }
    }
    while(notAppearsAtPath.size()){
        cout<<" b size of notAppearsAtPath: "<<notAppearsAtPath.size()<<endl;
        exit(0);
        safestPath sp(notAppearsAtPath);
        vector<pathCell*> temp = sp.find(start)->getPath();
        path.insert(path.end(), temp.begin(), temp.end());
        cout<<"size of path: "<<path.size()<<endl;
        cout<<"size of temp: "<<temp.size()<<endl;
        temp[temp.size()-1]->setAppear(true);
        splitBetweenRobots* sbr = new splitBetweenRobots();
        int index = sbr->findLocation(temp[temp.size()-1], notAppearsAtPath);
        notAppearsAtPath.erase(notAppearsAtPath.begin() + index);
        cout<<" b size of notAppearsAtPath: "<<notAppearsAtPath.size()<<endl; 
        start = temp[temp.size()-1]->getLocation();  
        temp.clear();
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

