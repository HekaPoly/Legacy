//Créé le 8 novembre 2017 par Charles
//Modifié le 08/11
//
//Branchement :
//Moteur
//Pin 9 Arduino - signal esc
//pin 4,7,8 Moteur - motor winding
//pin 6 moteur - GND

//Encoder
//Connection Encoder ---> SN75175
//Pin 1,4,9,10 Do not connect
//Pin 2 VCC
//Pin 3 GND
//Pin 5 Channel !A  ---> pin 1      R de 120ohm entre 1 et 2
//Pin 6 Channel A   ---> pin 2
//Pin 7 Channel !B  ---> pin 15     R de 120ohm entre 1 et 2
//Pin 8 Channel B   ---> pin 14

//SN75175                   ---> Teensy
//pin 4,12,16 ---> 5V
//pin 8       ---> GND
//pin 3       ---> Sorti A   ---> pin 2
//pin 4       ---> Sorti B   ---> pin 3

#include <Servo.h>

Servo esc;

const int escPin = 9;

void setup() {
  Serial.begin(9600);
  esc.attach(escPin);

  Serial.println("Waiting...");
  delay(2000);
  
  Serial.println("Arming... ");
  esc.write(0);
  delay(2000);
  Serial.println("Starting...");
}

void loop() {

esc.write(90);
/*
  for (int x = 50; x < 100; x += 10){
    esc.write(x);
    Serial.println(x);
    delay(1000);
    }

 for (int x = 100; x > 50; x -= 10){
    esc.write(x);
    Serial.println(x);
    delay(1000);
    }
*/
  
}
