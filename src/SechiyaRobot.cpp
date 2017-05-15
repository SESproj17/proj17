
#include <iostream>
#include <ros/ros.h>
#include <ses/RobotStatus.h>
#include "moveRobot.h"
using namespace std;


int robot_id;
int MAX_ROBOTS_NUM;
bool teamReady = false;

ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;

void publishReadyStatus();
void teamStatusCallback(const ses::RobotStatus::ConstPtr& status_msg);
void waitForTeam();

int main(int argc, char **argv)
{


    if (argc < 2) {
        ROS_ERROR("You must specify robot id.");
        return -1;
    }

    char *id = argv[1];
    robot_id = atoi(id);


    string node_name = "Robot_nu._";
    node_name += id;


    char *x = argv[2];
    int firstStart = atoi(x);

    char *y = argv[3];
    int secondStart = atoi(y);


    ros::init(argc, argv, node_name);
    ros::NodeHandle nh;



    nh.getParam("group_size",MAX_ROBOTS_NUM);
    // Check that robot id is between 0 and MAX_ROBOTS_NUM
    if (robot_id > MAX_ROBOTS_NUM || robot_id < 0 ) {
        ROS_ERROR("The robot's ID must be an integer number between 0 an 19");
        return -1;
    }
    ROS_INFO("Starting robot %d", robot_id);

    // Create a unique node name




    publishReadyStatus();

    waitForTeam();

    moveRobot move(firstStart,secondStart,robot_id);
    move.start();


    return 0;
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
    status_msg.is_ready = true;

    // Wait for the publisher to connect to subscribers
    sleep(1.0);
    team_status_pub.publish(status_msg);

    ROS_INFO("Robot %d published ready status", robot_id);
}

void waitForTeam()
{
    ros::Rate loopRate(1);

    // Wait until all robots are ready...
    while (!teamReady) {
        ROS_INFO("Robot %d: waiting for team", robot_id);
        ros::spinOnce();
        loopRate.sleep();
    }
}


