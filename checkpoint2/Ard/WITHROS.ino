#define R1     10  //IN1
#define R2     11   //IN2
#define L1     8   //IN3
#define L2     9    //IN4
#define EN_R    6   //ENA
#define EN_L    5  //ENB

#include <ros.h>
#include <stdio.h>
#include <std_msgs/String.h>

int pwml,pwmr;
ros::NodeHandle  nh;
void Callbackl( const std_msgs::String& receive_msg){
  String mess = receive_msg.data;
  pwml = atoi(mess.c_str()); 
  if (pwml>=0)
    LeftForward(pwml);
  else{
    pwml*=-1;
    LeftBack(pwml); 
  }
}

void Callbackr( const std_msgs::String& receive_msg){
  String mess = receive_msg.data;
  pwmr = atoi(mess.c_str());
  if (pwmr>=0)
    RightForward(pwmr);
  else{
    pwmr*=-1;
    RightBack(pwmr);
  }
}

ros::Subscriber<std_msgs::String> subl("ard_getl", Callbackl );
ros::Subscriber<std_msgs::String> subr("ard_getr", Callbackr );

void LeftBack(int pwm){
  analogWrite(EN_L,pwm);    
  digitalWrite(L2,HIGH);
  digitalWrite(L1,LOW);
}

void LeftForward(int pwm){
  analogWrite(EN_L,pwm);    
  digitalWrite(L2,LOW);
  digitalWrite(L1,HIGH);
}

void RightBack(int pwm){
  analogWrite(EN_R,pwm);    
  digitalWrite(R2,HIGH);
  digitalWrite(R1,LOW);
}
void RightForward(int pwm){
  analogWrite(EN_R,pwm);    
  digitalWrite(R2,LOW);
  digitalWrite(R1,HIGH);
}

void setup()
{
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(R1,OUTPUT);
  pinMode(R2,OUTPUT);
  pinMode(EN_L,OUTPUT);
  pinMode(EN_R,OUTPUT);
  nh.initNode();
  nh.subscribe(subl);
  nh.subscribe(subr);
  Serial.begin(57600);
}

void loop()
{
  nh.spinOnce();
  delay(100);
}
