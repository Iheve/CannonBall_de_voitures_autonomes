#include <Servo.h>

/* Constant used to communicate over the serial */
#define BAUDRATE 9600

/* Constants which define pins used by servos */
#define STEERING_SERVO_PIN (7)
#define PROPULSION_SERVO_PIN (8)

/* Constants which define pins used by RC channel */
//#define STERRING_CHANNEL_PIN (5)
//#define PROPULSION_CHANNEL_PIN (6)

#define INITIAL_SERVO_POSITION (90)
#define ACCELERATE_STEP (5)

/* Variable declaration */
Servo SteeringServo;
Servo PropulsionServo;
int loop_counter = 0;

void setup() {
    /* Output */
    pinMode(STEERING_SERVO_PIN, OUTPUT);
    pinMode(PROPULSION_SERVO_PIN, OUTPUT);
    pinMode(13, OUTPUT);

    SteeringServo.attach(STEERING_SERVO_PIN);
    SteeringServo.write(INITIAL_SERVO_POSITION);

    PropulsionServo.attach(PROPULSION_SERVO_PIN);
    PropulsionServo.write(INITIAL_SERVO_POSITION);

    Serial.begin(BAUDRATE);
    Serial.println("Setup finished !");
}

byte smoothAccelerate(byte current, byte target) {
    if (current >= target) {         //Brake
        current = target;
    } else if (loop_counter % 1000 == 0) { //Accelerate
        current = ((current + ACCELERATE_STEP) >= target) ?
                target : current + ACCELERATE_STEP;
    }

    return current;
}

void loop() {
    byte propulsionCurrent = INITIAL_SERVO_POSITION;
    byte steeringTarget = INITIAL_SERVO_POSITION,
         propulsionTarget = INITIAL_SERVO_POSITION;

    for(;;) {
        /* Get data from serial, wait for 2 bytes */
        if (Serial.available() >= 2) {
            steeringTarget = Serial.read();
            propulsionTarget = Serial.read();

        }

        SteeringServo.write(steeringTarget);
        PropulsionServo.write(
                smoothAccelerate(propulsionCurrent, propulsionTarget)
                );

        ++loop_counter;
    }
}
