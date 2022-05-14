#include <Servo.h>//Using servo library to control ESC
Servo esc; //Creating a servo class with name as esc
void setup()
{
esc.attach(9); //Specify the esc signal pin,Here as D8
//esc.writeMicroseconds(1100); //initialize the signal to 1000
esc.writeMicroseconds(2000);
delay(2000);
esc.writeMicroseconds(0);
delay(2000);

Serial.begin(9600);
}
void loop()
{
int val; //Creating a variable val
//val= analogRead(A0); //Read input from analog pin a0 and store in val
//val= map(val, 0, 1023,1000,2000); //mapping val to minimum and maximum(Change if needed) 
esc.writeMicroseconds(1200); //using val as the signal to esc
}
