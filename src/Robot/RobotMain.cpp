# include <unistd.h>
# include <time.h>
#include <iostream>
#include <signal.h>
#include <ros/ros.h>
#include <ses/RobotStatus.h>
#include "moveRobot.h"
using namespace std;


int robot_id;
int firstStart;
int secondStart;
int MAX_ROBOTS_NUM;
bool teamReady = false;

ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;

void publishReadyStatus();
void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg);
void waitForTeam();
void handler(int signum);
void publishAliveStatus();

int main(int argc, char **argv)
{

    if (argc < 2) {
        ROS_ERROR("You must specify robot id.");
        return -1;
    }
    char *lizinumber = argv[1];
    string lizi( lizinumber );
    cout << "RobotMain::main liziNumber = " << lizi << endl;
    
    char *id = argv[2];
    robot_id = atoi(id);


    string node_name = "Robot_";
    node_name += id;


    char *x = argv[3];
    firstStart = atoi(x);

    char *y = argv[4];
    secondStart = atoi(y);


    ros::init(argc, argv, node_name);
    ros::NodeHandle nh;

    team_status_pub = nh.advertise<ses::RobotStatus>("team_status", 10);
    team_status_sub = nh.subscribe("team_status", 1, &teamStatusCallback);



    nh.getParam("group_size",MAX_ROBOTS_NUM);
    // Check that robot id is between 0 and MAX_ROBOTS_NUM
    if (robot_id > MAX_ROBOTS_NUM || robot_id < 0 ) {
        ROS_ERROR("The robot's ID must be an integer number between 0 an 19");
        return -1;
    }
    ROS_INFO("Starting robot %d", robot_id);

    publishReadyStatus();

    waitForTeam();

    alarm(1);
    signal(SIGALRM, handler);


    moveRobot move(firstStart,secondStart,robot_id,lizi);
    
    move.start();


    return 0;
}


void handler(int signum) {
    publishAliveStatus();
    alarm(1);
    signal(SIGALRM, handler);
}

void publishAliveStatus(){

    ses::RobotStatus status_msg;

    status_msg.header.stamp = ros::Time::now();
    status_msg.robot_id = robot_id;
    status_msg.is_ready = true;

    // Wait for the publisher to connect to subscribers
    sleep(1.0);
    team_status_pub.publish(status_msg);

    //ROS_INFO("Robot %d published Alive status", robot_id);
}


void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg)
{
    // Check if message came from monitor
    if (status_msg->header.frame_id == "monitor") {
        ROS_INFO("Robot %d: Team is ready. Let's move!", robot_id);
        teamReady = true;
    }
}



void publishReadyStatus()
{
    ses::RobotStatus status_msg;

    status_msg.header.stamp = ros::Time::now();
    status_msg.robot_id = robot_id;
    status_msg.start_x = firstStart;
    status_msg.start_y = secondStart;
    status_msg.is_ready = true;

    // Wait for the publisher to connect to subscribers
    sleep(1.0);
    team_status_pub.publish(status_msg);

    ROS_INFO("Robot %d published ready status", robot_id);
}

void waitForTeam()
{
    int count = 0;
    ros::Rate loopRate(1);

    // Wait until all robots are ready...
    while (!teamReady) {
        ROS_INFO("Robot %d: waiting for team", robot_id);
        ros::spinOnce();
        if (count == 5) {publishReadyStatus(); count = 0;}
        count++;
        loopRate.sleep();
    }
}


