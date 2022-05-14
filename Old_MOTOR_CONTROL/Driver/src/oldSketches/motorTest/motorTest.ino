/*
    Name:       motorTest.ino
    Created:	2019-03-19 10:30:45 AM
    Author:     PC-CHARLES\Charles

	Sketch pour tester les différents paramètres du moteurs

Entrer 'x' pour faire analogWrite(x) (x entre 0 et 2047)
Entrer '=x' pour set la valeur de l'encoder à x

Commandes pouvant être entrées "#___":
	Registres
setupRegisters : Write default registers
readRegisters : Print the values of the registers
readFault : Read the fault register

	Moteur
stop : Activate brake pin


	Encoder
resetAngle : Set encoder to 0

Pour changer les pins : "*__"
brake
reset
enable
direction


//À implementer
reset : reset de DRV

*/

#include <SPI.h>
#include <Encoder.h>

#pragma region "Variables et constantes" //{
//SPI communication
const byte READ = 0b10000000;
const byte WRITE = 0b00000000;
const byte faultReg = 0x2a;
const int maxSpeed = 480000; //Speed of the SPI (HZ)
const int CS = 10;

//Moteur
const int resetPin = 2;
const int enablePin = 3;
const int directionPin = 4;
const int brakePin = 5;
const int motorPin = 6;
const int lockPin = 7;
const int faultPin = 8;
const int encoderPin1 = 0, encoderPin2 = 1;

bool resetState = false;
bool enableState = false;
bool directionState = false;
bool brakeState = false;

//Contrôle du moteur
const int pulseMax = 2048;
const int pulseMin = 100;
const int turnCW = 1;
const int turnCCW = 2;

//Encoder
const int gearReduction = 1; //100 with gearbox, 1 without
const int encoderResolution = 4;
const int countsPerTurn = 800;

Encoder encoder(encoderPin2, encoderPin1);

bool toPrint = false;

#pragma endregion //}

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting");

	SPISetup();
	motorSetup();

}

void loop()
{
	double angle = 0;
	readSerial(angle);
}

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

void readFault() {
	if (digitalRead(faultPin) == HIGH) {
		Serial.println("Il y a une faute :(");
	}
	else
		Serial.print("Pas de faute");

	Serial.print("Le registre de faute est : ");
	Serial.println(readRegister(0x2A),BIN);
}

void SPISetup() {
	//SPI Setup
	pinMode(CS, OUTPUT);
	SPI.begin();
	writeDefaultRegisters();

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
	enableState = true;
	digitalWrite(4, HIGH); //DIR
	directionState = true;
	digitalWrite(5, LOW); //Brake
}

double readAngle(bool fullTurn) {
	long count;
	double angle;
	count = encoder.read() / encoderResolution;    //To count
	angle = double(count) / countsPerTurn / gearReduction * 360; //To get degrees

	if (fullTurn)
		checkIfFullTurn(angle);

	return angle;
}

void checkIfFullTurn(double& angle) {

	if (angle > 360) {
		encoder.write(0);
	}

	else if (angle < -0.0001) {
		encoder.write(3200 * gearReduction);
	}
}

#pragma region "Motor" //{

void setDirection(int direction) {
	if (direction == turnCCW) {
		directionState = true;
		digitalWrite(directionPin, HIGH);
	}
	else {
		directionState = false;
		digitalWrite(directionPin, LOW);
	}
}

#pragma endregion //}

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

		if (commande[0] == '*') {
			commande = commande.substring(1);
			Serial.print("Toggling pin ");
			Serial.println(commande);
			togglePin(commande);
		}

		else {
			int n = commande.toInt();  //convert readString into a number
			analogWrite(motorPin, n);
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

	if (commande == "togglePrint")
		toPrint = !toPrint;

	if (commande == "stop")
		digitalWrite(brakePin, HIGH);
}

//Commandes pouvant être exécuté

//Reset position if #reset is sent to the serial monitor
void resetAngle() {
	Serial.println("Angle reset to 0");
	encoder.write(0);
}

void togglePin(String pinName){
	if (pinName == "direction") {
		directionState = !directionState;
		Serial.print("Direction pin set to : ");
		Serial.println(directionState);
		digitalWrite(directionPin, directionState);
	}

	if (pinName == "brake") {
		brakeState = !brakeState;
		Serial.print("Brake pin set to : ");
		Serial.println(brakeState);
		digitalWrite(brakePin, brakeState);
	}

	if (pinName == "enable") {
		enableState = !enableState;
		Serial.print("Enable pin set to : ");
		Serial.println(enableState);
		digitalWrite(enablePin, enableState);
	}

	if (pinName == "reset") {
		resetState = !resetState;
		Serial.print("Reset pin set to : ");
		Serial.println(resetState);
		digitalWrite(resetPin, resetState);
	}
}

#pragma endregion //}

