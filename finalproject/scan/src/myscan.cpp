#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>
#include <sensor_msgs/LaserScan.h>

using namespace std;

void number_callback(const std_msgs::Int32& msg){
	ROS_INFO("Received [%d]", msg.data);
}

void get_laser_callback(const sensor_msgs::LaserScan &laser){
 
	cout <<"ROS Lidar Data"<<endl;
	cout << "0:"<<(laser.ranges[0]+laser.ranges[1]+laser.ranges[2]+laser.ranges[718]+laser.ranges[719])/5<< endl;
	cout << "180:"<<(laser.ranges[178]+laser.ranges[179]+laser.ranges[180]+laser.ranges[181]+laser.ranges[182])/5 << endl;
	cout << "360:"<<(laser.ranges[358]+laser.ranges[359]+laser.ranges[360]+laser.ranges[361]+laser.ranges[362])/5 << endl;
	cout << "540:"<<(laser.ranges[538]+laser.ranges[539]+laser.ranges[540]+laser.ranges[541]+laser.ranges[542])/5 << endl;

	cout <<"-----------------" << endl;
}

int main(int argc, char ** argv){

	ros::init(argc,argv,"demo_topic_subscriber");
	ros::NodeHandle n;
	//ros::Subscriber number_subscriber = node_obj.subscribe("/rpi2ard", 1, number_callback);
	
	ros::Subscriber laser_sub = n.subscribe("/scan", 10, get_laser_callback);

	ros::spin();
	return 0;
}