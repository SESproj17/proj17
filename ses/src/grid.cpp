#include "grid.h"
#include <nav_msgs/GetMap.h>

bool grid::isOn = false;
grid* grid::instance = NULL;



grid* grid::getInstance()
{
    if(! isOn)
    {
        instance = new grid();
        isOn = true;
        return instance;
    }
    else
    {
        return instance;
    }
}




grid::grid() {
	//this->initGrid(a);
    this->readMap();
    initGrid();
}

//return cell at (i,j)
pathCell* grid::getCellAt(int i,int j) {
	return ((this->cells[i])[j]);
}

//Initialize the grid
void grid::initGrid() {
	this->cells.resize(this->rows);
	for (int i = 0; i < this->rows; ++i) {
		cells[i].resize(this->cols);
	}
	cout << "START " << endl;
	int id = 0;
	for (int i = 0; i < this->rows; ++i) { 
		for (int j = 0; j < this->cols; ++j)
		{

			cells[i][j] = new pathCell(new myTuple(i,j),myGrid[i][j],id,-1);
			id++;
		}
	}
    /// Add the 1's cases.
    for (int i = 0; i < this->rows; ++i) { 
        for (int j = 0; j < this->cols; ++j)
        {
            if (i > 0) {
                cells[i][j]->addNeighbors(cells[i-1][j]);
            } if (i < (this->rows - 1)) {
                cells[i][j]->addNeighbors(cells[i+1][j]);
            } if (j > 0) {
                cells[i][j]->addNeighbors(cells[i][j-1]);
            } if (j < (this->cols - 1)) {
                cells[i][j]->addNeighbors(cells[i][j+1]);
            }
        }
    }

}

void grid::readMap() {
	int currCell = 0;
    ROS_INFO(" to become available");
	ros::NodeHandle nh;
    while (!ros::service::waitForService("static_map", ros::Duration(3.0))) {
        ROS_INFO("Waiting for service static_map to become available");
    }
    ros::ServiceClient mapClient = nh.serviceClient<nav_msgs::GetMap>("static_map");
    nav_msgs::GetMap::Request req;
    nav_msgs::GetMap::Response res;
    if (!mapClient.call(req, res)) {ROS_INFO("NOT FOUND\n");}
    const nav_msgs::OccupancyGrid& map = res.map;
    this->rows = map.info.height;
    this->cols = map.info.width;
    myGrid.resize(rows);
    for (int i = 0; i < rows; i++) {
        myGrid[i].resize(cols);   
    }
    cout << "start " << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            myGrid[i][j] = (float)(map.data[currCell])/100;
            currCell++;
        }
    }
    reduceMap();
}

vector<pathCell*> grid::dijkstra(int initI,int initJ,int goalI,int goalJ){
    int size = this->rows*this->cols;
    vector<pathCell*> thisCells;
    int count = 0;
    vector<pathCell*> path;
    pathCell* current;
    pathCell* init = cells[initI][initJ];
    pathCell* goal = cells[goalI][goalJ];
    thisCells.resize(size);
    int k = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            cells[i][j]->setCost(-1.0);
            cells[i][j]->setLastPathCell(NULL);
            thisCells[k] = cells[i][j];
            k++;
        }
    }

    init->setCost(0.0);
    while (thisCells.size() != 0) {
        current = getMinCost(thisCells);
        if (current->isEqual(goal)) {break;}
        thisCells = removeCell(thisCells,current);
        vector<pathCell*> neigh = current->getNeighbors();
        for (int i = 0; i < neigh.size(); ++i) {
            if (neigh[i] == NULL) { continue; }
            float temp = current->getCost() + neigh[i]->getProb();
            if (temp < neigh[i]->getCost() || neigh[i]->getCost() == -1.0) {
                neigh[i]->setCost(temp);
                neigh[i]->setLastPathCell(current);
            }
        }
    }
    while (!goal->isEqual(init)) {
        path.push_back(goal);
        goal = goal->getLastCell();
    }
    path.push_back(goal);
    return path;
}

pathCell* grid::getMinCost(vector<pathCell*> thisCells) {
    float min = rows*cols;
    pathCell* current = NULL;
    for (int i = 0; i < thisCells.size(); ++i) {
        if (thisCells[i]->getCost() <= min && thisCells[i]->getCost() != -1.0) {
            min = thisCells[i]->getCost();
            current = thisCells[i];
        }
    }
    return current;
}

vector<pathCell*> grid::removeCell(vector<pathCell*> thisCells,pathCell* current) {
    vector<pathCell*> newVector;
    for (int i = 0; i < thisCells.size(); ++i) 
    {
        if (!current->isEqual(thisCells[i])) {
            newVector.push_back(thisCells[i]);
        }
    }
    return newVector;
}

void grid::reduceMap() {
    bool flag = true,contFlag = true;
    int newI = 0,newJ = 0;

    int newWidth = int(cols/47);
    int newHeight = int(rows/47);
    vector< vector<float> > newGrid;
    newGrid.resize(newHeight);

    for (int i = 0; i < newHeight; i++) {
        newGrid[i].resize(newWidth);
    }    

    int a = 0,b = 0;
    for (int i = 0; i < rows; i += 47) {
        for (int j = 0; j < cols; j+= 47) {
            newGrid[a][b] = myGrid[i][j] + 0.01;
            cout << newGrid[a][b] << " ";
            b++; 
       }
       b = 0; 
       a++;
       cout << endl;
    }
    this->myGrid = newGrid;
    this->rows = newHeight;
    this->cols = newWidth;
    cout << " ROWSSSSSS " << rows << " COLSSSS " << cols << endl;
}

void grid::print() {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            if (cells[i][j]->getState() == Visited) {
                cout << "X";
            } else {
                cout << "O";
            }
        }
        cout << endl;
    }
}