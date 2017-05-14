#include "robot.h"
#include <vector>
#include <sstream>
#include <string>

using namespace std;


/////where in this file we erase????
robot::robot(int first, int second): location(myTuple m(first,second)){
	cout<<"at constructor!";
	this->state = idle;

}

void robot::setPath(string path){
	vector<string> spath = split(path, ' ');
	this->path.resize(spath.size());
	for (int i = 0; i < spath.size(); ++i)
	{
		vector<string> loc = split(spath[i],',');
		this->path[i] = new myTuple(string2int(loc[0]),string2int(loc[1]));
	}
	//reverse it
}

void robot::setArea(string area){
	this->area = area;
}

void robot::setState(State state){
	this->state = state;
}

State robot::getState(){
	return this->state;
}
myTuple robot::getLocation(){
	return this->location;
}
bool robot::isTheLast(){
	this->path.size()==0;
}

string robot::getArea(){
	return this->area;
}

int robot::string2int(string s){
	int x;
	stringstream convert(s);
	convert>>x;
	return x;
}

vector<string> robot::split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}
