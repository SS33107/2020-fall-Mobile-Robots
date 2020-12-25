/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <std_msgs/Int32.h>

ros::NodeHandle nh;

std_msgs::Int32 ard2rpi_msg;
std_msgs::Int32 rpi2ard_msg;
ros::Publisher ard2rpi("ard2rpi", &ard2rpi_msg);


void Send2rpi( const std_msgs::Int32& rpi2ard){
  ard2rpi_msg.data = rpi2ard.data *2;
  ard2rpi.publish( &ard2rpi_msg );
}
ros::Subscriber<std_msgs::Int32> rpi2ard("rpi2ard", &Send2rpi);



void setup()
{
  nh.initNode();
  nh.subscribe(rpi2ard);
  nh.advertise(ard2rpi);
}

void loop()
{
  
  nh.spinOnce();
  delay(1000);
}
