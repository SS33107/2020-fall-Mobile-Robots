#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "demo2_publisher");
	ros::NodeHandle node_obj;
	ros::Publisher Lmotor_publisher = node_obj.advertise<std_msgs::Int32>("/receive_msgL", 1);
	ros::Publisher Rmotor_publisher = node_obj.advertise<std_msgs::Int32>("/receive_msgR", 1);
	ros::Rate loop_rate(10);
	int number_count = 0;
	int velL=0;
	int velR=0;
    std::cout <<"input velL  velR"<<endl;
	while (ros::ok())
	{
		std_msgs::Int32 msgL;
		std_msgs::Int32 msgR;
		std::cin >> velL >> velR;
        msgL.data = velL;
        msgR.data = velR;
		Lmotor_publisher.publish(msgL);
		Rmotor_publisher.publish(msgR);
		ros::spinOnce();
		loop_rate.sleep();
		++number_count;
	}
	return 0;
}
