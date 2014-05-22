#include <Servo.h>

#define  baudrate 9600  // com port speed a changer en adéquation avec le cpp

Servo servoTest; //Servo pour les test
Servo direction; //Servo de la direction
Servo propulsion; //Servo controlant la propulsion

int j = 0;
bool detected,stopSignal = false;
int vit;
int vit_max = 70;
int size = 0;  // taille du marker coloré a tracker
int pos = 0;   // position du marker coloré depuis le milieu de l'image (camera)

//----------------------------------------------------------------
// utiles pour lire les données du port serie
short MSB = 0;  // pour construire un entier de 2 octets depuis le port serie en octet
short LSB = 0;  // pour construire un entier de 2 octets depuis le port serie en octet
int   MSBLSB = 0;  // pour construire un entier de 2 octets depuis le port serie en octet
//----------------------------------------------------------------



void setup(){
    // Brancher le servo de test à la pin 7
    // Attacher le servo de direction à la pin 8
    // Attacher le servo controlant la propulsion à la pin 9
    // Attention à l'initialisation des angles qui est fixée à 90 degrés par défaut.
    pinMode(7, OUTPUT);
    servoTest.attach(7);
    pinMode(8, OUTPUT);
    direction.attach(8);
    pinMode(9, OUTPUT);
    propulsion.attach(9); // attention  particulièrement à cette initialisation, garder les roues motrices en l'air par précaution
    propulsion.write(90);
    direction.write(90);

    Serial.begin(baudrate);        // connection au serial port
    Serial.println("Starting Cam-servo Face tracker");

}

bool tooFast(){
    vit = propulsion.read();
    if (vit < vit_max){ true; }
    else{ false; }
}

//permet d'arreter le vehicule
void stop(){
    propulsion.write(90);
}

// arrete completement la simulation
void finalStop(){
    int s;
    int	d;
    int	p;
    propulsion.write(90);
    direction.write(90);
    s = servoTest.attached();
    servoTest.detach();
    d = direction.attached();
    direction.detach();
    p = propulsion.attached();
    propulsion.detach();
}
// permet d'acccélérer
void accelerate(){
    int oldAngle;
    oldAngle = propulsion.read();
    propulsion.write(oldAngle-8);
}

// permet d'accélérer de façon plus douce (avec le temps d'attente en ms)
void accelerateSmooth(unsigned long time){
    int oldAngle;
    oldAngle = propulsion.read();
    propulsion.write(oldAngle - 8);
    delay(time);
}

// permet de decélérer
void decelerate(){
    int oldAngle;
    oldAngle = propulsion.read();
    propulsion.write(oldAngle+8);
}

// permet de decélérer de façon plus douce (avec le temps d'attente en ms)
void decelerateSmooth(unsigned long time){
    int oldAngle;
    oldAngle = propulsion.read();
    propulsion.write(oldAngle + 8);
    delay(time);
}

// permet de tourner les roues à droite suivant un angle (0-90)
void turnright(int angle){
    direction.write(90+angle);
}

// permet de tourner les roues à gauche suivant un angle (0-90)
void turnleft(int angle){
    direction.write(90-angle);
}


// la routine pour la recherche de markers
void routine(){
    int t = 5000; // temps pour faire une boucle suivant la vitesse
    int v = 82; // gère la vitesse de la voiture
    propulsion.write(v);
    //propulsion.writeMicroseconds(1456);
    direction.write(45);
    delay(t);
    direction.write(135);
    delay(t);
}



void loop() {
    accelerateSmooth(2000);

}
