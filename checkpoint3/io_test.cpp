#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>
#include "wiringPi.h"
#include <ctime>


using namespace std;
int LightPin=1;

//TODO
int TouchPinL=7;
int TouchPinR=27;
int TouchPinB=28;

#define COLLIDE 0 
#define BACK 1
#define FIND_LED 2
#define SPIN 3
#define FORWARD 4
#define END 5


#define WAIT 4
#define GO 5

std_msgs::Int32 msgL;
std_msgs::Int32 msgR;



void MoveStop(ros::Publisher& Lmotor_publisher, ros::Publisher& Rmotor_publisher){
	msgL.data = 0;
	msgR.data = 0;
	Lmotor_publisher.publish(msgL);
	Rmotor_publisher.publish(msgR);
}
void MoveSpin(ros::Publisher& Lmotor_publisher, ros::Publisher& Rmotor_publisher){
	msgL.data = 50;
	msgR.data = -50;
	Lmotor_publisher.publish(msgL);
	Rmotor_publisher.publish(msgR);
}
void MoveFoward(ros::Publisher& Lmotor_publisher, ros::Publisher& Rmotor_publisher){
	msgL.data = 50;
	msgR.data = 50;
	Lmotor_publisher.publish(msgL);
	Rmotor_publisher.publish(msgR);
}
void MoveBackward(ros::Publisher& Lmotor_publisher, ros::Publisher& Rmotor_publisher){
	msgL.data = -50;
	msgR.data = -50;
	Lmotor_publisher.publish(msgL);
	Rmotor_publisher.publish(msgR);
}



int main(int argc, char **argv)
{
	ros::init(argc, argv, "gpio_test_node");

	ros::NodeHandle node_obj;

	ros::Publisher Lmotor_publisher = node_obj.advertise<std_msgs::Int32>("/receive_msgL", 1);
	ros::Publisher Rmotor_publisher = node_obj.advertise<std_msgs::Int32>("/receive_msgR", 1);

	ros::Rate loop_rate(10);
	
	int number_count = 0,velL=0,velR=0;
    //setenv("WIRINGPI_GPIOMEM","1",1);
	wiringPiSetup();
	
	pinMode(LightPin,INPUT);
	pinMode(TouchPinL,INPUT);
	pinMode(TouchPinR,INPUT);
	pinMode(TouchPinB,INPUT);
	string STATUS_STRING[] = {"COLLIDE", "BACK", "FIND_LED", "SPIN", "FORWARD", "END"};

    int _light=0, _TouchPinL=0, _TouchPinR=0, _TouchPinB=0;
	int status = FORWARD;
	int last_state_number_count = 0;
	while (ros::ok())
	{
		_light=digitalRead(LightPin);
		_TouchPinL=digitalRead(TouchPinL);
		_TouchPinR=digitalRead(TouchPinR);
		_TouchPinB=digitalRead(TouchPinB);

		
		ROS_INFO("LightPin: %d",_light);
		ROS_INFO("TouchPinL: %d",_TouchPinL);
		ROS_INFO("TouchPinR: %d",_TouchPinR);
		ROS_INFO("TouchPinB: %d",_TouchPinB);
		ROS_INFO("number_count: %d", number_count);
		ROS_INFO("last_state_number_count: %d", last_state_number_count);

		ROS_INFO("status: %d, %s", status, STATUS_STRING[status].c_str());
		
		switch(status){
			case COLLIDE:
				MoveStop(Lmotor_publisher, Rmotor_publisher);
				status = BACK;
				last_state_number_count = number_count;
				break;
			case BACK:
				MoveBackward(Lmotor_publisher, Rmotor_publisher);
				if(number_count - last_state_number_count > 50){
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					status = FIND_LED;
					last_state_number_count = number_count;
				}
				break;
			case FIND_LED:
				MoveSpin(Lmotor_publisher, Rmotor_publisher);
				if(number_count - last_state_number_count > 60){
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					status = END;
				}
				if(_TouchPinL || _TouchPinR || _TouchPinB){
					status=COLLIDE;
				}
				break;
			case SPIN:
				break;
			case FORWARD:
				if(_TouchPinL || _TouchPinR || _TouchPinB){
					status=COLLIDE;
				}
				else{
					MoveFoward(Lmotor_publisher, Rmotor_publisher);
				}
				break;
			case END:
				MoveStop(Lmotor_publisher, Rmotor_publisher);
				status = END;
				break;
		}
		number_count ++;
        
		if(number_count > 400){
			MoveStop(Lmotor_publisher, Rmotor_publisher);
			break;
		}
		loop_rate.sleep();
	}
	return 0;
}


// #include "ros/ros.h"
// #include "std_msgs/Int32.h"
// #include <iostream>

// using namespace std;

// int main(int argc, char **argv)
// {
	// ros::init(argc, argv, "demo2_publisher");
	// ros::NodeHandle node_obj;
	// ros::Publisher Lmotor_publisher = node_obj.advertise<std_msgs::Int32>("/receive_msgL", 1);
	// ros::Publisher Rmotor_publisher = node_obj.advertise<std_msgs::Int32>("/receive_msgR", 1);
	// ros::Rate loop_rate(10); //0.1s
	// int number_count = 0;
	// int velL=0;
	// int velR=0;
    // std::cout <<"input velL  velR"<<endl;
	
	
	// int status = WAIT;
	// while (ros::ok())
	// {
		// ros::spinOnce();
		// loop_rate.sleep();
	// }
	// return 0;
// }

			// case WAITSTATE_NO_COLLIDE:
				// std::cout << "Pls enter two vel";
				// std::cin >> velL >> velR;
				// msgL.data = velL;
				// msgR.data = velR;
				// // msgL.data = 0;
				// // msgR.data = 0;
				// Lmotor_publisher.publish(msgL);
				// Rmotor_publisher.publish(msgR);
				
				// status = GO;
				// break;
			// case GO:
				// if(++number_count%10 == 0){
					// status = WAIT;
					// msgL.data = 0;
					// msgR.data = 0;
					// Lmotor_publisher.publish(msgL);
					// Rmotor_publisher.publish(msgR);
				// }
				// break;
