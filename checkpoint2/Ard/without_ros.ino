//The sample code for driving one way motor encoder
#include <PID_v1.h>
#include <stdio.h>

const byte encoder0pinAR = 4;//RA pin
const byte encoder0pinBR = 3;//RB pin 
const byte encoder0pinAL = 2;//LA pin 
const byte encoder0pinBL = 7;//LB pin 
const byte enableR = 6;
const byte enableL = 5;
const byte L1 = 8;
const byte L2 = 9;
const byte R1 = 10;
const byte R2 = 11;
byte encoder0PinALastR, encoder0PinALastL;
double durationR,durationL,abs_durationR,abs_durationL;//the number of the pulses
boolean DirectionR,DirectionL;//the rotation direction
boolean resultR,resultL;
double pwmL,pwmR;
double SetpointR,SetpointL;
double KpR=0.6, KiR=5, KdR=0,KpL=0.6, KiL=5,KdL=0;
PID myPIDR(&abs_durationR, &pwmR, &SetpointR, KpR, KiR, KdR, DIRECT);
PID myPIDL(&abs_durationL, &pwmL, &SetpointL, KpL, KiL, KdL, DIRECT);
char cmd[9] ;

void BackR(){
  analogWrite(enableR,abs(pwmR));    
  digitalWrite(R2,HIGH);
  digitalWrite(R1,LOW);
}

void ForwardR(){
  analogWrite(enableR,abs(pwmR));    
  digitalWrite(R2,LOW);
  digitalWrite(R1,HIGH);
}
void BackL(){
  analogWrite(enableL,abs(pwmL));    
  digitalWrite(L2,HIGH);
  digitalWrite(L1,LOW);
}

void ForwardL(){
  analogWrite(enableL,abs(pwmL));    
  digitalWrite(L2,LOW);
  digitalWrite(L1,HIGH);
}



void setup()
{  
   Serial.begin(9600);
   pinMode(enableR,OUTPUT);   //L298P Control port settings DC motor driver board for the output mode
   pinMode(enableL,OUTPUT);
   myPIDR.SetMode(AUTOMATIC);//PID is set to automatic mode
   myPIDR.SetSampleTime(50);//Set PID sampling frequency is 100ms
   myPIDL.SetMode(AUTOMATIC);//PID is set to automatic mode
   myPIDL.SetSampleTime(50);//Set PID sampling frequency is 100ms
   EncoderInit();//Initialize the module
   SetpointR=0;
   SetpointL=0;
}

void loop()
{     Serial.println(atof(cmd));
      Serial.readBytes(cmd,9);
      if(atof(cmd)>=0){
        ForwardR();
        ForwardL();
        SetpointR=atof(cmd);
        SetpointL=atof(cmd);
        }
       else{
        BackR();
        BackL();
        SetpointR=(-1)*atof(cmd);
        SetpointL=(-1)*atof(cmd);
        }
      abs_durationR=abs(durationR);
      abs_durationL=abs(durationL);
      resultR=myPIDR.Compute(); 
      resultL=myPIDL.Compute();//PID conversion is complete and returns 1
      Serial.print("right:");
      Serial.print(pwmR);
      Serial.print(" ; left:");
      Serial.println(pwmL);
      if(resultR){
        durationR = 0;
       }
        if(resultL){
        durationL = 0;
       }
      delay(50);
}

void EncoderInit()
{
  DirectionR = true;//default -> Forward
  DirectionL = true;//default -> Forward
  pinMode(encoder0pinBR,INPUT);
  pinMode(encoder0pinBL,INPUT);
  attachInterrupt(1, wheelSpeedR, CHANGE);
  attachInterrupt(0, wheelSpeedL, CHANGE);
}

void wheelSpeedR()
{
  int LstateR = digitalRead(encoder0pinAR);
  if((encoder0PinALastR == LOW) && LstateR==HIGH)
  {
    int valR = digitalRead(encoder0pinBR);
    if(valR == LOW && DirectionR)
    {
      DirectionR = false; //Reverse
    }
    else if(valR == HIGH && !DirectionR)
    {
      DirectionR = true;  //Forward
    }
  }
  encoder0PinALastR = LstateR;

  if(!DirectionR)  durationR++;
  else  durationR--;
}
void wheelSpeedL()
{
  int LstateL = digitalRead(encoder0pinAL);
  if((encoder0PinALastL == LOW) && LstateL==HIGH)
  {
    int valL = digitalRead(encoder0pinBL);
    if(valL == LOW && DirectionL)
    {
      DirectionL = false; //Reverse
    }
    else if(valL == HIGH && !DirectionL)
    {
      DirectionL = true;  //Forward
    }
  }
  encoder0PinALastL = LstateL;

  if(!DirectionL)  durationL++;
  else  durationL--;
}
