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

subArea::subArea(int rows,int cols,vector<pathCell*> givenCells,float givenProb,int lvl){
    this->prob = givenProb;
    this->myLevel = lvl;
    this->state = NotAssigned;
    this->cells = givenCells;
    this->myCells.resize(rows);
    for (int i = 0; i < rows; ++i)
    {
        myCells[i].resize(cols);
    }
    for (int i = 0; i < givenCells.size(); ++i)
    {
        pathCell* c = givenCells[i];
        c->setArea(this);
        myCells[c->getLocation().returnFirst()][c->getLocation().returnSecond()] = c;
    }
    this->notFoundYet = 0;
    for (int i = 0; i < givenCells.size(); ++i)
    {
        if(givenCells[i]->getState() == NotVisited){
            this->notFoundYet++;
        }
    }
}

//output : all uncovered subareas of this area
vector<subArea*> subArea::getInheritance(){
    splitBetweenRobots* sbr = new splitBetweenRobots();
    vector<subArea*> connectedList;
    vector<pathCell*> graph = getCells();
    vector<bool> inherLocalVisited(graph.size(), false);

    for (int i = 0; i < graph.size(); ++i)
    {
        if (graph[i]->getState()==NotVisited && !inherLocalVisited[i])
            {
                vector<pathCell*> fromDfs = dfs(graph[i]);
                for (int j = 0; j < fromDfs.size(); ++j)
                {
                    int index = sbr->findLocation(fromDfs[j], graph);
                    inherLocalVisited[index] = true;
                }

                subArea* connectedArea = new subArea(myCells.size(),myCells[0].size(),fromDfs,prob,myLevel);
                connectedArea->changeState(NotAssigned);
                vector<pathCell*> cells = connectedArea->getCells();
                connectedList.push_back(connectedArea);
                //unnessesry hopfully....
                for (int k = 0; k < cells.size(); ++k)
                {
                    cells[k]->setArea(connectedArea);
                }
                //

                //debug code
                //connectedArea->print();
                //cout<<endl;
                //debug code
            }    
    }
    //cout<<"dfs: num of areas at the inheritance: "<<connectedList.size()<<endl;
    return connectedList;
}

vector<pathCell*> subArea::dfs(pathCell* start){
    vector<bool> dfsLocalVisited(cells.size(), false);
    //cout<<"dfs:cells.size() "<<cells.size()<<endl;
    //cout<<"dfs:dfsLocalVisited.size() "<<dfsLocalVisited.size()<<endl;
    splitBetweenRobots* sbr = new splitBetweenRobots();
    vector<pathCell*> connectedSubGraph;
    stack<pathCell*> stack;
   
    pathCell* s = start;
    stack.push(s);
    while (!stack.empty()){
        s = stack.top();
        stack.pop();
        int index = sbr->findLocation(s, cells);
        if(s->getState() == NotVisited && !dfsLocalVisited[index]){
            cout<<index<<endl;
            connectedSubGraph.push_back(s);
            //cout<<"dfs:connectedSubGraph.size() "<<connectedSubGraph.size()<<endl;

            dfsLocalVisited[index] = true;
            vector<pathCell*>neib = s->getNeighbors();
            for (int i =0; i< neib.size();i++){
                index = sbr->findLocation(neib[i], cells);
                if (index>-1)
                {
                    stack.push(neib[i]);
                }
            }
        }
    }
    return connectedSubGraph;
}
    

void subArea::print() {
    for (int i = 0; i < myCells.size(); ++i)
    {
        for (int j = 0; j < myCells[i].size(); ++j)
        {
            if(myCells[i][j] == NULL){
                cout << "O ";
            }else {
                if(myCells[i][j]->getState() == NotVisited){
                    cout << "1 ";
                }else if(myCells[i][j]->getState() == Visited){
                    cout << "2 ";
                }else{
                    cout << "? ";
                }
                
            }
        }
        cout << endl;
    }    
}

int subArea::getLeftCells() { 
    return this->notFoundYet; 
}


/*
greedy approach, where in each step it leads the robot
to the safest nearest cell to its current location which has not been
covered yet.
*/
vector<pathCell*> subArea::coverge(myTuple start){
    //print();
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
    bool first = true;
    while(notAppearsAtPath.size()){
        safestPath sp(notAppearsAtPath);
        vector<pathCell*> temp = sp.find(start)->getPath();
        if(first){path.insert(path.end(), temp.begin(), temp.end());}
        else{path.insert(path.end(), temp.begin()+1, temp.end());}   
        temp[temp.size()-1]->setAppear(true);
        splitBetweenRobots* sbr = new splitBetweenRobots();
        int index = sbr->findLocation(temp[temp.size()-1], notAppearsAtPath);
        notAppearsAtPath.erase(notAppearsAtPath.begin() + index);
        start = temp[temp.size()-1]->getLocation();  
        temp.clear();
        first = false;
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
    if (this->notFoundYet > 0)this->notFoundYet--;
    if (this->notFoundYet == 0){this->state = Covered;}
}

