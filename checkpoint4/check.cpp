#include "ros/ros.h"
#include <wiringPi.h>
#include <iostream>
#include <std_msgs/Int16.h>
//light receive pin 3
int lightpin = 29;
double beacon=0;
ros::Time previous_time;
double lastms,current;
int IR_data=0,cycle=10000,now=0;
double count_0=0,count_1=0;
int main (int argc, char **argv){
  ros::init(argc, argv, "light_receive_data");
  ros::NodeHandle n;
  ros::Publisher light_pub = n.advertise<std_msgs::Int16>("light_data", 1);
  unsigned short int light_rev = 0;
  std_msgs::Int16 light_data;
   //use this command whithout sudo
  setenv("WIRINGPI_GPIOMEM", "1", 1);
  //library setup function
  wiringPiSetup () ;
  pinMode (lightpin, INPUT) ;
  //10hz
  ros::Rate loop_rate(25);
  while(ros::ok()){
  lastms=ros::Time::now().toSec();
   while (ros::Time::now().toSec()-lastms<0.04){
     IR_data = digitalRead(lightpin);
     if(IR_data == 0)
          count_0++;
     else if(IR_data == 1)
          count_1++;
    }
    beacon = (count_0)/(count_1+count_0);
    ROS_INFO("beacon read : %1f",beacon);
    count_0=0;
    count_1=0;  
    now=0;
   loop_rate.sleep();
  }
  return 0 ;
}