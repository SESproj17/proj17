#include <iostream>
#include <ros/ros.h>
#include "grid.h"
#include "Robot.h"
using namespace std;

int main(int argc, char **argv) {
	ros::init(argc, argv, "ses");
	ROS_INFO("#WE_BEGAN");

	grid* g = grid::getInstance();
	vector<pathCell*> p = g->dijkstra(0,0,3,3);
	cout << "length   " << p.size() << endl;
    Robot* robot = new Robot();
    // Start the movement
    g->print();
    robot->work(p);
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    g->print();
    /*
	grid *g;
	g =  grid::getInstance();
	*/

	return 0;
}


    
    
    
    
         
    
    
         
    