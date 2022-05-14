/*
Cr�� le 18/03/2019
Auteurs : Charles et Maude

Code pour controller un moteur en mode slave par I2C

Commandes pouvant �tre entr�es "#___": 
	Moteur
resetAngle : Set encoder to 0
readRegisters : Print the values of the registers
stop : Activate brake pin

	PID
togglePrint : Affiche les valeurs du PID ou non
setkp : Pour changer la valeur de kp
setki : Pour changer la valeur de ki
setkd : Pour changer la valeur de kd

Autres :
Entrer '=x' : Set encoder value to x
Entrer 'x' : Set goal to x

*/
#define ENCODER_OPTIMIZE_INTERRUPTS

#include <WireKinetis.h>
#include <Wire.h>
#include <SPI.h>
#include <PID_v1.h>
#include <Encoder.h>

/*
#pragma region "Prototypes" //{
//Fonctions SPI
void writeRegister(byte reg, byte value1, byte value2);
unsigned int readRegister(byte reg);
void SPISetup();
void motorSetup();
void readRegisters();


//Fonctions qui fonctionnent :
double readAngle(bool fullTurn);
void updateAngle(double newAngle, double& angle);
void resetAngle();
void checkIfFullTurn(double& angle);

void readSerial(double& angle);
void executeCommande(String commande);

#pragma endregion //}*/

#pragma region "Variables et constantes" //{

//SPI communication
const byte READ = 0b10000000;
const byte WRITE = 0b00000000;
const byte faultReg = 0x2a;
const int maxSpeed = 480000; //Speed of the SPI (HZ)
const int CS = 10;
const byte registersAdress[] = {0x00,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x2A };
//const byte defaultValues[] = {0x0111,0x04FF,0x6800,0x00D7,0x0000,0x0000,0x0000,0x0000,0x0000,0xF000,0x0000,0x0000 };


//Moteur
const int resetPin = 2;
const int enablePin = 3;
const int directionPin = 4;
const int brakePin = 5;
const int motorPin = 6;
const int lockPin = 7;
const int faultPin = 8;
const int encoderPin1 = 1, encoderPin2 = 0;

//Lecture de l'encoder
const double gearReduction = 1; //100 with gearbox, 1 without
const double encoderResolution = 4;
const double countsPerTurn = 800;

//Contr�le du moteur
const int pulseMax = 2047;
const int pulseMin = 100;
const int turnCW = 1;
const int turnCCW = 2;


//Conraintes sur les angles max et min
const int angleMax = 400;
const int angleMin = -10;

//PID
double setpoint, input, output;
double kp = 0.5;
double ki = 0;
double kd = 0;
double goal = 10000; //Variation in angle to go
bool autoPilot = false;
bool goingUp = true;	//Direction of autopilot
bool toPrint = true;

PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);		//PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, Direction) Option to allow Proportional on measurment
IntervalTimer timer;
Encoder encoder(encoderPin2, encoderPin1);
double speed = 0;
double position = 0;

#pragma endregion //}


void toggleGoal()
{
	goal *= -1;
}

void setup() {
	Serial.begin(9600);
	Serial.println("Starting");

  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  
	SPISetup();
  //I2CSetup();
	//motorSetup();
	//PIDSetup();


	timer.begin(toggleGoal, 500000);

  
}

/*
void binaryControl()
{
	readSerial(goal);
	setpoint = goal;
	position = encoder.read();
	int delta = position - setpoint;
	setDirection(delta < 0);
	if (abs(delta) > 100)
	{
		// digitalWrite(brakePin, LOW);
		analogWrite(motorPin, 400);
	}
	else 
	{
		analogWrite(motorPin, 0);
		// digitalWrite(brakePin, HIGH);
		Serial.println(position);
	}
		

}*/

void loop() {
	//binaryControl();
  //readFault();
  //delay(1000);
  //readRegisters();
  Serial.println(encoder.read());
	//pidControl();
	//  setDirection(turnCW);
	//  analogWrite(motorPin, 500);
	//  Serial.println(readAngle(speed));
}

#pragma region "Setup and SPI" //{
void writeRegister(byte reg, byte value1, byte value2) {

	byte toSend = WRITE | reg;

	SPI.beginTransaction(SPISettings(maxSpeed, MSBFIRST, SPI_MODE0));
	digitalWrite(CS, HIGH);
	delayMicroseconds(1);

	//send write command + reg adress
	SPI.transfer(toSend);

	//send first 8 bits (MSB)
	SPI.transfer(value1);
	//send last 8 bits (LSB)
	SPI.transfer(value2);

	digitalWrite(CS, LOW);
	SPI.endTransaction();
}

unsigned int readRegister(byte reg) {
	byte toRead = READ | reg;

	SPI.beginTransaction(SPISettings(maxSpeed, MSBFIRST, SPI_MODE0));
	digitalWrite(CS, HIGH);
	delayMicroseconds(1);
	//send read command + reg adress
	SPI.transfer(toRead);

	int result;
	byte readValue;

	//get first 8 bits of data (MSB)
	readValue = SPI.transfer(0x00);
	result = readValue << 8;
	//get last 8 bits of data (LSB)
	readValue = SPI.transfer(0x00);

	//combine into 16 bits result
	result = result | readValue;

	digitalWrite(CS, LOW);
	SPI.endTransaction();

	return (result);
}

void readRegisters() {
	//To Read the registers
	Serial.println("Lecture des registres");
	Serial.println(readRegister(0x00), HEX);
	Serial.println(readRegister(0x02), HEX);
	Serial.println(readRegister(0x03), HEX);
	Serial.println(readRegister(0x04), HEX);
	Serial.println(readRegister(0x0A), HEX);
	Serial.println();
	Serial.println();
}

void SPISetup() {
	//SPI Setup
	pinMode(CS, OUTPUT);
	SPI.begin();
	writeRegisters();
	//Recommended setup
  
}

void writeRegisters() {
	writeRegister(0x00, 0x01, 0x11);
	//writeRegister(0x01,0x00,0x00);
	writeRegister(0x02, 0x04, 0xFF);
	writeRegister(0x03, 0x68, 0x00);
	writeRegister(0x04, 0x00, 0xD7);
	writeRegister(0x05, 0x00, 0x00);
	writeRegister(0x06, 0x00, 0x00);
	writeRegister(0x07, 0x00, 0x00);
	writeRegister(0x08, 0x00, 0x00);
	writeRegister(0x09, 0x00, 0x00);
	writeRegister(0x0A, 0xF0, 0x00);
	writeRegister(0x0B, 0x00, 0x00);
	writeRegister(0x2A, 0x00, 0x00);
}

void readFault() {
	if (digitalRead(faultPin) == HIGH) {
		Serial.println("Il y a une faute :(");
	}
	else
		Serial.print("Pas de faute");

	Serial.print("Le registre de faute est : ");
	Serial.println(readRegister(0x2A), BIN);
}

void writeDefaultRegisters() {
	//Recommended setup
	writeRegister(0x00, 0x01, 0x11);
	//writeRegister(0x01,0x00,0x00);
	writeRegister(0x02, 0x04, 0xFF);
	writeRegister(0x03, 0x68, 0x00);
	writeRegister(0x04, 0x00, 0xD7);
	writeRegister(0x05, 0x00, 0x00);
	writeRegister(0x06, 0x00, 0x00);
	writeRegister(0x07, 0x00, 0x00);
	writeRegister(0x08, 0x00, 0x00);
	writeRegister(0x09, 0x00, 0x00);
	writeRegister(0x0A, 0xF0, 0x00);
	writeRegister(0x0B, 0x00, 0x00);
	writeRegister(0x2A, 0x00, 0x00);
}

/*
void checkRegisters() {
	bool registersOk = true;
	for (int i = 0; i < 12; i++) {
		if (readRegister(registersAdress[i] != defaultValues[i]))
			registersOk = false;
	}
	if (registersOk)
		Serial.println("Les registres ont �t� bien �crit");
	else
		Serial.println("Erreur lors de l'�criture des registres");
}
*/

/*
void motorSetup() {
	//Motor Setup
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT); //Brake
	pinMode(6, OUTPUT); //Motor
	pinMode(7, INPUT);
	pinMode(8, INPUT);

	analogWriteResolution(11);
	analogWriteFrequency(motorPin, 23437.5); //Values between 0-2047

	digitalWrite(2, LOW); //RESET
	digitalWrite(3, HIGH); //Enable
	digitalWrite(4, HIGH); //DIR
	digitalWrite(5, LOW); //Brake
}

void PIDSetup() {
	input = 0;
	setpoint = 0;
	myPID.SetMode(AUTOMATIC);
	myPID.SetOutputLimits(-2047, 2047);
}

#pragma endregion //}

#pragma region "Encoder" //{

//TESTER : Pour old controller, Fonction qui retourne l'angle du moteur, If fullTurn = true, will convert the angle to a circle (0-360), false for no correction

double countToAngleFactor = 360.0 / (countsPerTurn*gearReduction*encoderResolution);
double readAngle(bool fullTurn) {
	double count;
	//double angle;
	count = encoder.read();    //To count
	double angle =  encoder.read()*countToAngleFactor ; //To get degrees

	//if (fullTurn)
	//	checkIfFullTurn(angle);

	return angle;
}

//Fonction pour reset la position � chaque tour
//TODO: s'assurer qu'il n'y aille plus d'angle n�gatif en v�rifiant avant de corriger
void checkIfFullTurn(double& angle) {

	if (angle > 360) {
		encoder.write(0);
	}

	else if (angle < -0.0001) {
		encoder.write(3200 * gearReduction);
	}
}

//Fonction pour updater l'angle s'il change et l'afficher
void updateAngle(double newAngle, double& angle) {
	if (newAngle != angle) {
		Serial.print("Angle = ");
		Serial.println(newAngle);
		angle = newAngle;
	}
}

#pragma endregion //}

#pragma region "Motor" //{

void setDirection(bool direction){
	if (direction)
		digitalWrite(directionPin, HIGH);
	else
		digitalWrite(directionPin, LOW);
}

#pragma endregion //}

#pragma region "PID" //{


void pidControl() {
	//readSerial(goal);
	setpoint = goal;
	position = encoder.read();

	//Update input with the Angle
	input = goal - position;

	//Compute 
	myPID.Compute();

	//Check the direction to turn
	if (output > 0) {
		setDirection(turnCCW);
		speed = output;
	}
	else if (output < 0) {
		setDirection(turnCW);
		speed = -output;
	}
	 Serial.print(position);
	 Serial.print("\tsp: ");
	 Serial.println(setpoint);
	//if (toPrint)	
	//	printVariables();

	analogWrite(motorPin, speed);
}

//Fonction pour changer les variables
void setTunings(int kp, int ki, int kd) {
	myPID.SetTunings(kp, ki, kd);
	Serial.print("Kp= ");
	Serial.print(kp);
	Serial.print("	Ki= ");
	Serial.print(ki);
	Serial.print("	Kd= ");
	Serial.println(kd);
}

//To print the variables : setpoint, input, output, speed
void printVariables() {
	Serial.print("Setpoint : ");
	Serial.print(setpoint);
	Serial.print(" Position : ");
	Serial.print(position);
	Serial.print("	Input : ");
	Serial.print(input);
	Serial.print("	Output : ");
	Serial.print(output);
	Serial.print("	Speed : ");
	Serial.print(speed);
	Serial.print(" Direction :");
	Serial.println(digitalRead(directionPin));
}

#pragma endregion//}

#pragma region "Serial Communication" //{
//Read data from the serial port. Return the value if its a number. start with # to call a function, =x to set angle to x, x to set goal new goal
void readSerial(double& angleToGo) {

	String commande;

	while (Serial.available()) {
		char c = Serial.read();  //gets one byte from serial buffer
		commande += c; //makes the string readString
		delay(2);  //slow looping to allow buffer to fill with next character
	}

	if (commande.length() > 0) {
		Serial.println(commande);  //so you can see the captured string 

		if (commande[0] == '#') {
			commande = commande.substring(1);
			Serial.print("Commande : ");
			Serial.println(commande);
			executeCommande(commande);
		}

		if (commande[0] == '=') {
			double angle = double(commande.substring(1).toInt());
			double count = 0;
			count = angle / 360 * countsPerTurn*gearReduction*encoderResolution;
			encoder.write(count);
		}

		else {
			int n = commande.toInt();  //convert readString into a number
			angleToGo = n;
			Serial.print("Angle To go : ");
			Serial.println(angleToGo);
		}
		commande = ""; //empty for next input
	}
}

//Choose the commande to execute, #reset reset les angles;  #setkp, #setkd ou #setki to change one of those parameters
void executeCommande(String commande) {

	if (commande == "reset")
		resetAngle();

	if (commande == "readRegisters")
		readRegisters();

	if (commande == "setupRegisters")
		writeDefaultRegisters();

	if (commande == "readFault")
		readFault();

	if (commande == "togglePrint")
		toPrint = !toPrint;

	if (commande == "stop")
		digitalWrite(brakePin, HIGH);

	if (commande == "autopilot") {
		autoPilot = !autoPilot;
		if (autoPilot)
			Serial.println("Auto Pilot on");
		else
			Serial.println("Auto Pilot off");
	}

	if (commande == "setkp") {
		goal = 0;
		Serial.println("Enter kp : ");
		while (!Serial.available()) {}

		commande = "";
		while (Serial.available()) {
			char c = Serial.read();  //gets one byte from serial buffer
			commande += c; //makes the string readString
			delay(2);  //slow looping to allow buffer to fill with next character
		}
		kp = commande.toInt();  //convert readString into a number
		setTunings(kp, ki, kd);
	}

	if (commande == "setki")
	{
		goal = 0;		Serial.println("Enter ki : ");
		while (!Serial.available()) {}

		commande = "";
		while (Serial.available()) {
			char c = Serial.read();  //gets one byte from serial buffer
			commande += c; //makes the string readString
			delay(2);  //slow looping to allow buffer to fill with next character
		}
		ki = commande.toInt();  //convert readString into a number
		setTunings(kp, ki, kd);
	}

	if (commande == "setkd")
	{
		goal = 0;
		Serial.println("Enter kd : ");
		while (!Serial.available()) {}

		commande = "";
		while (Serial.available()) {
			char c = Serial.read();  //gets one byte from serial buffer
			commande += c; //makes the string readString
			delay(2);  //slow looping to allow buffer to fill with next character
		}
		kd = commande.toInt();  //convert readString into a number
		setTunings(kp, ki, kd);
	}

}


//Commandes pouvant �tre ex�cut�

//Reset position if #reset is sent to the serial monitor
void resetAngle() {
	Serial.println("Angle reset to 0");
	encoder.write(0);
}

#pragma endregion //}

void I2CSetup() {
  	Wire.begin(1); // A CHANGER!!!!
  	Wire.onReceive(onReceiveEvent);
	//Wire.onRequest(onRequestEvent);
}

void onReceiveEvent(int receivedCount) {
	String commande;

	while (Wire.available()) {
		char c = Wire.read();
		commande += c;
		delay(2);
	}

	if (commande.length() > 0) {
		Serial.println(commande);  //so you can see the captured string 

		if (commande[0] == '#') {
			commande = commande.substring(1);
			Serial.print("Commande : ");
			Serial.println(commande);
			executeCommande(commande);
		}

		if (commande[0] == '=') {
			double angle = double(commande.substring(1).toInt());
			double count = 0;
			count = angle / 360 * countsPerTurn*gearReduction*encoderResolution;
			encoder.write(count);
		}

		else {
			goal = commande.toInt();  //convert readString into a number
			Serial.print("Angle To go : ");
			Serial.println(goal);
		}
		commande = ""; //empty for next input
	}
}

void onRequestEvent() {
	
}*/
