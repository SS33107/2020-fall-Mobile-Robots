#define R1     10  //IN1
#define R2     11   //IN2
#define L1     8   //IN3
#define L2     9    //IN4
#define EN_R    6   //ENA
#define EN_L    5  //ENB

#include <stdio.h>

int pwml,pwmr;


void LeftBack(int pwm){
  digitalWrite(L2,HIGH);
  digitalWrite(L1,LOW);
  analogWrite(EN_L,pwm);    
}

void LeftForward(int pwm){
  digitalWrite(L2,LOW);
  digitalWrite(L1,HIGH);
  analogWrite(EN_L,pwm);    
  
}

void RightBack(int pwm){
  digitalWrite(R2,HIGH);
  digitalWrite(R1,LOW);
  analogWrite(EN_R,pwm);    
}
void RightForward(int pwm){
  digitalWrite(R2,LOW);
  digitalWrite(R1,HIGH);
  analogWrite(EN_R,pwm);    
}

void setup()
{
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(R1,OUTPUT);
  pinMode(R2,OUTPUT);
  pinMode(EN_L,OUTPUT);
  pinMode(EN_R,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  RightForward(0);
  LeftForward(0);
  delay(100);
}
