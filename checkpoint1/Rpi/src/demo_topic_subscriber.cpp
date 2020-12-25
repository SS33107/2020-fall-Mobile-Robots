#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>

void number_callback(const std_msgs::Int32& msg){
	ROS_INFO("Received [%d]", msg.data);
}
int main(int argc, char ** argv){

	ros::init(argc,argv,"demo_topic_subscriber");
	ros::NodeHandle node_obj;
	ros::Subscriber number_subscriber = node_obj.subscribe("/ard2rpi", 1, number_callback);
	ros::spin();
	return 0;
}