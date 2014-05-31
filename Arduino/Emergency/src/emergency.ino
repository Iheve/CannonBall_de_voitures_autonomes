//Simple program used to generate an emergency stop
//BE CAREFUL ! LINK THE GROUND OF THE RC RECEIVER WITH THE ARDIUNO ONE
//OTHERWISE IT WILL NOT WORK
//See pictures for details

#include <Servo.h>
#define  baudrate 9600 

Servo propulsion;

int inPin = 7; 
int motorPin = 8; 
int val = 0;//variable to store the read value

void setup()
{
  Serial.begin(9600);//setup serial
  pinMode(inPin, INPUT);//sets the digital pin 7 as input

  //Let's launch the motor
  propulsion.attach(motorPin);
  propulsion.write(88);
}

void loop()
{
  
  int ch1 = pulseIn(inPin, HIGH, 25000); // Read the pulse width of the channel

  Serial.print("Channel 1:"); // Print the value of 
  Serial.println(ch1);        // each channel

  if(ch1 < 1700) { //if we accelerate, STOP everything
    propulsion.write(91); 
  } 
  if(ch1 > 2100) { //if we brake, launch the prog again
    propulsion.write(88); 
  } 

  delay(100);//For console output

}
