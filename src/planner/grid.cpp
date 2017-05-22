#include "grid.h"


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
    ros::NodeHandle node;
    ros::Subscriber sub;
    sub = node.subscribe("threat_map", 1, &grid::readMap, this);
    ros::Rate rate(10);
    this->rows = -1;
    this->cols = -1;    
    ros::spinOnce();
    int count = 1;
    while (ros::ok()) {        
        if (this->rows != -1) {break;}
        cout << " count  " << count << endl;
        ros::spinOnce(); // Need to call this function often to allow ROS to process incoming messages
        rate.sleep();        
        count++;
    }

    
	//this->initGrid(a);
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

void grid::readMap(const nav_msgs::OccupancyGrid& map) {
	int currCell = 0;
    ROS_INFO(" to become available");
	ros::NodeHandle nh;
    this->rows = map.info.height;
    this->cols = map.info.width;
    myGrid.resize(rows);
    for (int i = 0; i < rows; i++) {
        myGrid[i].resize(cols);   
    }
    cout << "start " << endl;
    for (int i = rows - 1; i >= 0; i--) {
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
    if (goalI > this->rows || goalJ > this->cols) {return thisCells;}
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
    reverse(path.begin(),path.end());
    return path;
}

pathCell* grid::getMinCost(vector<pathCell*> thisCells) {
    float min = rows*cols;
    pathCell* current = NULL;
    for (int i = 0; i < thisCells.size(); ++i) {
        if (thisCells[i]->getProb() == 1.0) { continue; }
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

    int blockSize = 60;

    int newWidth = int(cols/blockSize);
    int newHeight = int(rows/blockSize);
    vector< vector<float> > newGrid;
    newGrid.resize(newHeight);

    for (int i = 0; i < newHeight; i++) {
        newGrid[i].resize(newWidth);
    }    

    int a = 0,b = 0;
    for (int i = 0; i < rows; i += blockSize) {
        for (int j = 0; j < cols; j+= blockSize) {
            newGrid[a][b] = myGrid[i][j] + 0.01;
            
            b++; 
       }
       b = 0; 
       a++;

    }
    this->myGrid = newGrid;
    this->rows = newHeight;
    this->cols = newWidth;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int temp = myGrid[i][j]*100;
            int temp1 = temp%10;
            if (temp1 != 0) {
                temp = temp + (10 - temp1);
            }
            myGrid[i][j] = ((float)temp) / 100.0;
            cout <<  myGrid[i][j] << " ";
       }
        cout << endl;
    }
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
int grid::getRows() { return this->rows; }
int grid::getCols() { return this->cols; }