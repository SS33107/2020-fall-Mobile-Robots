//The sample code for driving one way motor encoder
const byte encoder0pinAR = 4;//RA pin
const byte encoder0pinBR = 3;//RB pin -> the digital pin 3
const byte encoder0pinAL = 2;//LA pin -> the interrupt pin 0
const byte encoder0pinBL = 7;//LB pin -> the digital pin 3
byte encoder0PinALastR, encoder0PinALastL;
int durationR,durationL;//the number of the pulses
boolean DirectionR,DirectionL;//the rotation direction


void setup()
{
  Serial.begin(9600);//Initialize the serial port
  EncoderInit();//Initialize the module
}

void loop()
{
  Serial.print("right:");
  Serial.print(durationR);
  Serial.print(" ; left:");
  Serial.println(durationL);
  durationR = 0;
  durationL = 0;
  delay(100);
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
