#include <Servo.h>

#define  baudrate 9600 

Servo propulsion; 

int spin_motor_max = 20; //motor max speed
int spin_motor_min = 90; //motor min speed
//Brake value between 91 (soft brake) and 180 (hard brake)
int spin_motor_stop = 91; //Wheels not spinning

int spin_motor = spin_motor_stop;

void setup(){
    propulsion.attach(9); //Motor on pin 9 (pwm)
    propulsion.write(spin_motor);
    Serial.begin(9600);
}

void accelerate(){
    propulsion.write(--spin_motor);
}

void decelerate(){
    propulsion.write(++spin_motor);
}

void setSpeed(int speed) {
    //TODO verif
    spin_motor=speed;
    propulsion.write(speed);
}

int brake=0;

void loop() {
    //Accelerate
    for (int i=0; i<10; i++) {
        spin_motor-=5;
        setSpeed(spin_motor);
        delay(500);
    }

    setSpeed(spin_motor_stop);

    //Simple brake test
    for (int i=0; i < 10; i++) {
        Serial.println("VROOM!");
        setSpeed(spin_motor_max);
        delay(1000);
        setSpeed(90+brake);
        Serial.println(90+brake);
        Serial.println(propulsion.read());
        brake+=10;
        delay(3000);
    }

    setSpeed(spin_motor_stop);
}
