//Simple program used to generate an emergency stop
//BE CAREFUL ! LINK THE GROUND OF THE RC RECEIVER WITH THE ARDIUNO ONE
//OTHERWISE IT WILL NOT WORK
//See pictures for details

#include <Servo.h>
#define  baudrate 9600 

Servo propulsion;

//RC Receiver pin 3
int motorPin = 8; 

unsigned long time=0;
unsigned long last_time=0;
unsigned long period=0;

void rc_handler() {
  //BE CAREFUL, DO NOT PRINT STUFF HERE
  //Serial.println use interrupts too, thus resulting in a deadlock :(

  time = micros(); //CAREFUL, micro will overflow after 70min (returning 0)
  period = time - last_time;
  if (period < 1000) {
    //We've got an emergency, stop everything
    propulsion.write(91);
  }

  //Currently not working
  /*
  else if (period > 1900) {
    propulsion.write(88);
  }
  */

  last_time = time;
}

void setup()
{
  Serial.begin(9600);//setup serial

  //Let's launch the motor
  propulsion.attach(motorPin);
  propulsion.write(88);

  attachInterrupt(1, rc_handler, CHANGE); //1 is pin 3 (0 pin 2)
}

void loop()
{
    //debug purpose
    Serial.println(period);
}
