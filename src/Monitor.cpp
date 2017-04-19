
#include "ros/ros.h"
#include "RobotStatus.h"
#include "Monitor.h"

Monitor::Monitor(int teamSize)
{
	robotsCount = 0;
	teamSize = teamSize;

	// Check that robot id is between 0 and MAX_ROBOTS_NUM
	if (teamSize > MAX_ROBOTS_NUM || teamSize < 1 ) {
	    ROS_ERROR("The team size must be an integer number between 1 and %d", MAX_ROBOTS_NUM);
	    return -1;
	}

	ros::init(argc, argv, "monitor");
	ros::NodeHandle nh;

	team_status_pub = nh.advertise<multi_sync::RobotStatus>("team_status", 10);
	team_status_sub = nh.subscribe("team_status", 1, &teamStatusCallback);

	ROS_INFO("Waiting for robots to connect...");

	ros::spin();
}

//take care of alarms in monitor and robots

void Monitor::teamStatusCallback(const multi_sync::RobotStatus::ConstPtr& status_msg)
{
	int robot_id = status_msg->robot_id;

	if (!robotsReady[robot_id]) {
		ROS_INFO("Robot %d is ready!\n", robot_id);
		robotsReady[robot_id] = true;
		robotsCount++;

		if (robotsCount == teamSize) {
			ROS_INFO("All robots GO!");

			multi_sync::RobotStatus status_msg;

			status_msg.header.stamp = ros::Time::now();
			status_msg.header.frame_id = "monitor";

			team_status_pub.publish(status_msg);
		}
	}
}
