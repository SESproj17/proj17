#include <iostream>
#include <ros/ros.h>
#include "grid.h"
#include "Robot.h"
using namespace std;

int main(int argc, char **argv) {
	ros::init(argc, argv, "ses");
	ROS_INFO("#WE_BEGAN");

	grid* g = grid::getInstance();
	
	/*

    algo1* al = new algo1();

    vector<area*> vc = al->make_areas(0.1,0.4);
    cout<<"subs:"<<endl;
    /*for(int i=0; i < vc.size(); i++){
        vc[i]->print();
    }*/
    //vector<subArea*> ca = al->getConnectedAreas(vc);

    //cout<<"subs:"<<endl;
   /* for(int i=0; i < ca.size(); i++){
        ca[i]->print();
    }*/
     //cout<<"hellllllllllllllllllllo!"<<endl;
	

	vector<pathCell*> p = g->dijkstra(0,0,4,4);    

	cout << "length   " << p.size() << endl;
	for (int i = 0; i < p.size(); ++i) {
		myTuple loc = p[i]->getLocation();
		cout<<"x: "<<loc.returnFirst()<<", y: "<<loc.returnSecond()<<endl;

	}

    g->print();

    Robot* robot = new Robot();
    // Start the movement
    robot->work(p);
    cout << " HEREEE  " << endl;
    //cout << endl;
    //cout << endl;
    //cout << endl;
    //cout << endl;
    //cout << endl;
    //g->print();
    /*
	grid *g;
	g =  grid::getInstance();
	*/

	return 0;
}


    
    
    
    
         
    
    
         
    