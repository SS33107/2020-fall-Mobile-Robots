#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>
#include "wiringPi.h"
#include <ctime>
#include <csignal>

using namespace std;
int LightPin=4;

//TODO
int TouchPinL=7;
int TouchPinR=27;
int TouchPinB=28;

#define COLLIDE 0 
#define BACK 1
#define FIND_LED 2
#define SPIN 3
#define SPINFIX 6
#define FORWARD 4
#define END 5

#define ON_LIGHT 0
#define OFF_LIGHT 1

#define WAIT 4
#define GO 5

std_msgs::Int32 msgL;
std_msgs::Int32 msgR;
ros::Publisher Lmotor_publisher ;
ros::Publisher Rmotor_publisher ;


void MoveStop(ros::Publisher& Lmotor_publisher, ros::Publisher& Rmotor_publisher){
	msgL.data = 0;
	msgR.data = 0;
	Lmotor_publisher.publish(msgL);
	Rmotor_publisher.publish(msgR);
}
void MoveSpin(ros::Publisher& Lmotor_publisher, ros::Publisher& Rmotor_publisher){
	msgL.data = -25;
	msgR.data = 25;
	Lmotor_publisher.publish(msgL);
	Rmotor_publisher.publish(msgR);
}
void MoveSpinRev(ros::Publisher& Lmotor_publisher, ros::Publisher& Rmotor_publisher){
	msgL.data = -15;
	msgR.data = 15;
	Lmotor_publisher.publish(msgL);
	Rmotor_publisher.publish(msgR);
}
void MoveFoward(ros::Publisher& Lmotor_publisher, ros::Publisher& Rmotor_publisher){
	msgL.data = 130;
	msgR.data = 130;
	Lmotor_publisher.publish(msgL);
	Rmotor_publisher.publish(msgR);
}
void MoveBackward(ros::Publisher& Lmotor_publisher, ros::Publisher& Rmotor_publisher){
	msgL.data = -80;
	msgR.data = -80;
	Lmotor_publisher.publish(msgL);
	Rmotor_publisher.publish(msgR);
}


// void signalHandler( int signum ) {
	// cout << "Interrupt signal (" << signum << ") received.\n";
	// MoveStop(Lmotor_publisher, Rmotor_publisher);
	// // cleanup and close up stuff here  
	// // terminate program  

	// exit(signum);  
// }


int main(int argc, char **argv)
{
	// signal(SIGINT, signalHandler);  
	ros::init(argc, argv, "gpio_test_node");

	ros::NodeHandle node_obj;

	Lmotor_publisher = node_obj.advertise<std_msgs::Int32>("/receive_msgL", 1);
	Rmotor_publisher = node_obj.advertise<std_msgs::Int32>("/receive_msgR", 1);

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
    int L_light=0, L_TouchPinL=0, L_TouchPinR=0, L_TouchPinB=0, L_status=0;
	
	int status;
	if(argc == 2)
		status = END;
	else
		status = FORWARD;
		
	int last_state_number_count = 0;
	int SPIN_random, randomed=0;
	
	MoveStop(Lmotor_publisher, Rmotor_publisher);
	cout << "init status " << STATUS_STRING[status].c_str() << endl;
	
	while (ros::ok())
	{
		L_light = _TouchPinL;
		L_TouchPinL = _TouchPinL;
		L_TouchPinR = _TouchPinR;
		L_TouchPinB = _TouchPinB;
		L_status = status;
		
		_light=digitalRead(LightPin);
		_TouchPinL=digitalRead(TouchPinL);
		_TouchPinR=digitalRead(TouchPinR);
		_TouchPinB=digitalRead(TouchPinB);
		
		if(argc == 2){
			ROS_INFO("LightPin: %d",_light);
			ROS_INFO("TouchPinL: %d",_TouchPinL);
			ROS_INFO("TouchPinR: %d",_TouchPinR);
			ROS_INFO("TouchPinB: %d",_TouchPinB);
		}
		if ( L_light != _TouchPinL || L_TouchPinL != _TouchPinL || 
		L_TouchPinR != _TouchPinR || L_TouchPinB != _TouchPinB || 
		status != L_status ){		
			ROS_INFO("LightPin: %d",_light);
			ROS_INFO("TouchPinL: %d",_TouchPinL);
			ROS_INFO("TouchPinR: %d",_TouchPinR);
			ROS_INFO("TouchPinB: %d",_TouchPinB);
			ROS_INFO("number_count: %d", number_count);
			ROS_INFO("last_state_number_count: %d", last_state_number_count);
			ROS_INFO("status: %d, %s", status, STATUS_STRING[status].c_str());
		}
		if( _TouchPinB ){
			status=END;
		}
		switch(status){
			case COLLIDE:
				if(number_count - last_state_number_count > 3){
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					status = FIND_LED;
					last_state_number_count = number_count;
				}
				else{
					cout << "In COLLIDE" << endl;
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					status = BACK;
					last_state_number_count = number_count;
				}
				break;
			case BACK:
				if(number_count - last_state_number_count > 13){
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					status = FIND_LED;
					last_state_number_count = number_count;
				}
				else{
					cout << "Inside BACK" << endl;
					MoveBackward(Lmotor_publisher, Rmotor_publisher);
				}
				break;
			case FIND_LED:

				if(_light == ON_LIGHT){
					cout << "FIND LED!!" << endl;
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					
					last_state_number_count = number_count;
					status = SPINFIX;
					cout << "Turn to SPINFIX" << endl;
				}
				else if(_TouchPinL || _TouchPinR){
					cout << "Collide while spin" << endl;
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					last_state_number_count = number_count;
					status = COLLIDE;
					cout << "Turn to COLLIDE" << endl;
				}
				else if(number_count - last_state_number_count > 50){
					cout << "CANNOT FIND LED" << endl;
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					last_state_number_count = number_count;
					status = SPIN;
					cout << "Turn to SPIN" << endl;
				}
				else{
					MoveSpin(Lmotor_publisher, Rmotor_publisher);
					status = FIND_LED;
				}
				break;
			case SPINFIX:
				if(number_count - last_state_number_count > 5){
					cout << "REV finish" << endl;
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					last_state_number_count = number_count;
					status = FORWARD;
					cout << "Turn to FORWARD" << endl;
				}
				else{
					MoveSpin(Lmotor_publisher, Rmotor_publisher);
				}
				break;
			case SPIN:
				if(_TouchPinL || _TouchPinR){
					cout << "Collide while Random spin" << endl;
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					last_state_number_count = number_count;
					status = COLLIDE;
					cout << "Turn to COLLIDE" << endl;
				}

				if( !randomed){
					SPIN_random = rand()%15+1;
					randomed = 1;
				}
				cout << "Random Spin " << SPIN_random << endl;
				MoveSpin(Lmotor_publisher, Rmotor_publisher);
				if(number_count - last_state_number_count > SPIN_random){
					MoveStop(Lmotor_publisher, Rmotor_publisher);
					last_state_number_count = number_count;
					status = FORWARD;
					cout << "Turn to FORWARD" << endl;
					randomed = 0;
				}
				break;
			case FORWARD:
				if(_TouchPinL || _TouchPinR ){
					last_state_number_count = number_count;
					status=COLLIDE;
					cout << "Forward Collide"<< endl;
				}
				else if(number_count - last_state_number_count > 80){
					last_state_number_count = number_count;
					status=FIND_LED;
					cout << "Turn to FINDLED" << endl;
				}
				else{
					MoveFoward(Lmotor_publisher, Rmotor_publisher);
				}
				break;
			case END:
				cout << "ENDDDDDDDDDDDDDDD" << endl;
				MoveStop(Lmotor_publisher, Rmotor_publisher);
				status = END;
				break;
		}
		number_count ++;
        
		if(number_count > 1000){
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
