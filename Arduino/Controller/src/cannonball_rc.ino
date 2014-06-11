#include <Servo.h>

/* Constant used to communicate over the serial */
#define BAUDRATE 115200

/* Constants which define pins used by servos */
#define STEERING_SERVO_PIN (5)
#define THROTTLE_SERVO_PIN (6)
#define LED_PIN (13)

/* Variable declaration */
Servo SteeringServo;
Servo ThrottleServo;
int steeringTarget=90;
int throttleTarget=91;

unsigned long time=0;
unsigned long last_time=0;
unsigned long period=0;
boolean emergency = false;

void rc_handler() {
    //BE CAREFUL, DO NOT PRINT STUFF HERE
    //Serial.println use interrupts too, thus resulting in a deadlock :(

    time = micros(); //CAREFUL, micro will overflow after 70min (returning 0)
    period = time - last_time;
    if (period < 1000) {
        //We've got an emergency, stop everything
        emergency = true;
    }
    last_time = time;
}
void setup() {
    /* Output */
    pinMode(STEERING_SERVO_PIN, OUTPUT);
    pinMode(THROTTLE_SERVO_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);
    SteeringServo.attach(STEERING_SERVO_PIN);
    ThrottleServo.attach(THROTTLE_SERVO_PIN);

    attachInterrupt(1, rc_handler, CHANGE); //1 is pin 3 (0 pin 2)

    Serial.begin(BAUDRATE);
    Serial.println("Setup finished !");
}

void loop() {
    if (Serial.available() >= 2) {
        steeringTarget = Serial.read();
        throttleTarget = Serial.read();
        Serial.flush();
    }
    SteeringServo.write(steeringTarget);
    ThrottleServo.write(throttleTarget);

    if (emergency) {
        digitalWrite(13, HIGH);
        ThrottleServo.write(89);    //Prevent the car to go back
        for (;;)
            ThrottleServo.write(130);
    }
}
