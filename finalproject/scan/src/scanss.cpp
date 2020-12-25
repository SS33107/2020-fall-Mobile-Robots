
#include <ros/ros.h>  
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sensor_msgs/LaserScan.h>

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"

#define MAX_DIRECTIONS 72
#define RAD2DEG(x) ((x)*180./M_PI)

using namespace std;

std_msgs::Int32 msgL;
std_msgs::Int32 msgR;
ros::Publisher Lmotor_publisher;
ros::Publisher Rmotor_publisher;

#define MID_BLOCK 1
#define MID_GO 2
#define FINISH 3
int result[MAX_DIRECTIONS];
float min_[MAX_DIRECTIONS];
int camera_ret[5];

void get_camera_callback(const std_msgs::Int32MultiArray::ConstPtr& array){
	int i = 0;
	// print all the remaining numbers
	for(std::vector<int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it){
		camera_ret[i] = *it;
		i++;
	}
	//cout << "Camera ret " << camera_ret[0] << "/" << camera_ret[1] << endl;
}

void get_laser_callback(const sensor_msgs::LaserScan::ConstPtr& scan) {

    int count = scan->scan_time / scan->time_increment;

	float a[MAX_DIRECTIONS];
	int b[MAX_DIRECTIONS];
	
	for(int i =0;i<MAX_DIRECTIONS;i++){
		a[i] = 0;
		b[i] = 0;
		min_[i] = 1000;
	}
	
	for(int i = 0; i < count; i++) {
        float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
		if(degree < 0){
			degree += 360;
		}
		//cout << degree << endl;
		float empty_pnt = isinf ( scan->ranges[i]) ? 8 : scan->ranges[i];
		int t = int(degree/ (360/MAX_DIRECTIONS));
		
		a[ t ] += empty_pnt;
		
		//b[ int(degree/ (360/MAX_DIRECTIONS)) ] ++;
		if(min_[t] > empty_pnt){
			min_[t] = empty_pnt;
		}
    }
}


int main(int argc, char** argv)
{
    // Initialize ROS node
    ros::init(argc, argv, "laser");
    ros::NodeHandle n;
    Lmotor_publisher = n.advertise<std_msgs::Int32>("/receive_msgL", 1);
    Rmotor_publisher = n.advertise<std_msgs::Int32>("/receive_msgR", 1);
    ros::Rate loop_rate(10);
    // Subscribe to the ros topic in gazebo and set the callback function
    ros::Subscriber laser_sub = n.subscribe("/scan", 10 , get_laser_callback);
	ros::Subscriber camera_sub = n.subscribe("/point_x_scale", 10 , get_camera_callback);
	
	float avoid_distance;
	avoid_distance = atof(argv[1]);
	int move_unit = atoi(argv[2]);
	int obj_size = atoi(argv[3]);
	int ignore_block = 0;

	int status = MID_GO;
    while (ros::ok())
    {
		cout << "--------------------------------- " << endl;
		int final_direction = 0, min_distance = 10000, min_index = -1;
		int current_distance;
		int forward_section = MAX_DIRECTIONS/2;
		int start_section = 36;
		int middle_section = 54;
		int mid_interval = 5;
		int end_section = 72;
		bool middle_block = false;
		bool midblock_flag = false;
		
		for(int i = middle_section - mid_interval; i <  middle_section + mid_interval + 1 ; i++){
			cout << i* (360/MAX_DIRECTIONS) << ":" <<min_[i]<< ":"<< avoid_distance <<endl;
			if(min_[i] < avoid_distance)
				midblock_flag = true;
		}

		int object_center_in_width = camera_ret[1] < 1000 ? -1: camera_ret[0];
		int object_width = camera_ret[1];
		int camera_width = 640;
		int divide = 10;

		if(camera_ret[1] > obj_size){
			ignore_block = 1;
		}
		if(camera_ret[1] > obj_size*3){
			status = FINISH;
		}


		switch(status){
			case FINISH:
				msgL.data = 0;
				msgR.data = 0;
				status = FINISH;
				break;
			case MID_BLOCK:
				cout << "MID_BLOCK" <<endl;
				if(midblock_flag){
					msgL.data = -15;
					msgR.data = 15;
					status = MID_BLOCK;					
				}
				else{
					status = MID_GO;
				}
				break;
			case MID_GO:
				cout << "MID_GO" <<endl;
				if(ignore_block == 0 && midblock_flag){
					status =  MID_BLOCK ;
				}
				else{
					//Search target in the carmera;

					int part = int ( object_center_in_width / (camera_width / divide));
					cout << "Camera ret " << camera_ret[0] << "/" << camera_ret[1] << "/" << obj_size << endl;
					if(object_center_in_width == -1){
						msgL.data = 50;
						msgR.data = 50;
						cout << "Search target fail , Go straight: " << msgL.data << "/" << msgR.data <<endl;
					}
					else{
						msgR.data = (divide - part + 1) * move_unit;
						msgL.data = (part + 1) * move_unit;	
						cout << "Part " << part <<endl;
						cout << "By Search target, Go L/R: " << msgL.data << "/" << msgR.data <<endl;
					}
					status = MID_GO;
				}
				break;
		}

		if(argc > 4){
			//debug mode, don't publish th msg to motor.
		}
		else{
			Rmotor_publisher.publish(msgR);
			Lmotor_publisher.publish(msgL);
		}

        ros::spinOnce();
        loop_rate.sleep();
		// callback rotation
    }
    return 0;
}
