//Créé le 22 janvier 2018 par Charles
//Modifié le 22/01
//Code pour controller le moteur avec un PID

//TO DO Avant CQI
//Tout tester


#pragma region "Branchements" //{
//À faire :
//	Encoder
//Brancher encoder, fil rouge à gauche
//Brancher dans breadbord : Fil 1 dans F1, 6 dans E1
//	ESC
//Power -> 20V
//Les 2 GND -> GND du générateur
//Blanc -> 9 du teensy
//Noir -> Gnd du breadboard

//Moteur
//Pin 9 Arduino - signal esc
//pin 4,7,8 Moteur - motor winding
//pin 6 moteur - GND

//Encoder
//**fil rouge est la pin 2
//1, 4, 9, 10 Do not connect
//2 → VCC
//3 → GND
//5 (Channel !A)	→ pin 1 (SN75175)
//6 (Channel A) 	→ pin 2 (SN75175)
//7 (Channel !B)	→ pin 15 (SN75175)R de 120 ohm entre 1 et 2
//8 (Channel B)	→ pin 14 (SN75175)
//
//SN75175
//4, 12, 16 → 5V
//8 → GND
//3 (Sortie A)	→ pin 4 (logic shifter)
//13 (Sortie B) 	→ pin 3 (logic shifter)
//R de 120 ohm entre 1 et 2
//R de 120 ohm entre 14 et 15
//
//Logic shifter (SN74LVC245A)
//1   → 5v
//16 (Sortie B) 	→ pin 2 (Teensy)
//17 (Sortie A) 	→ pin 3 (Teensy)
//19 → GND
//20 → 3.3V

#pragma endregion //}

#include <PID_v1.h>
#include <Servo.h>
#include <Encoder.h>


#pragma region "Prototypes" //{
//Fonctions qui fonctionnent :
double readAngle(Encoder& encoder);
void resetAngle(Encoder& encoder);

void readSerial(double& angle, Encoder& encoder, Servo& servo);
void armEsc(Servo& moteur);
void executeCommande(String commande, Encoder& encoder, Servo& servo);

#pragma endregion //}


#pragma region "Constantes" //{

const int escPin = 9;
const int encoderPin1 = 2, encoderPin2 = 3;

//Constantes pour lecture de l'encoder
const int gearReduction = 100; //Change to 100 when the gearbox is installed
const int encoderResolution = 4;
const int countsPerTurn = 800;

//Constantes pour valeurs de l'esc
const int pulseMiddle = 1500;
const int pulseMaxCCW = 1000;
const int pulseMaxCW = 2000;

//Contraintes sur les angles max et min, à implementer
const int angleMax = 400;
const int angleMin = -10;

bool armed = false;

#pragma endregion	//} 

//Variables pour le PID
double setpoint, input, output;

double kp = 10;
double ki = 0.1;	
double kd = 0;
double goal = 0; //Variation in angle to go
double oldGoal = 0; //Previous goal
bool autoPilot = false;
bool goingUp = true;	//Direction of autopilot
double positionIni = 0;
double position = 0;
double speed = 0;

PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);		//PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, Direction) Option to allow Proportinal on measurment

Encoder encoder1(encoderPin2, encoderPin1);
Servo moteur1;


void setup() {
	Serial.begin(9600);
	moteur1.attach(escPin);
	Serial.println("Starting");

	//Setup du PID
	input = 0;
	setpoint = 0;
	myPID.SetMode(AUTOMATIC);
	myPID.SetOutputLimits(-500, 500);
}

void loop() {
	readSerial(goal, encoder1, moteur1);	//Read data from serial monitor, update the angle to go

	if(!autoPilot)
		pidControl();
	else
		autoControl();
}


#pragma region "Functions" //{

#pragma region "PID" //{
//Entrer une variation d'angle: -:CCW, +:CW
void pidControl() {

	position = readAngle(encoder1);

	//If the goal has changed
	if (goal != oldGoal) {
		Serial.print("New Goal, moving ");
		Serial.print(goal);
		setpoint = goal;
		positionIni = position;
		oldGoal = goal;
	}

	//Update input with the Angle
	input = positionIni - position;

	//Compute 
	myPID.Compute();

	speed = 1500 + output;

	printVariables();

	moteur1.writeMicroseconds(speed);
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

//To print the variables
void printVariables() {
	Serial.print("Setpoint : ");
	Serial.print(setpoint);
	Serial.print("	Input : ");
	Serial.print(input);
	Serial.print("	Output : ");
	Serial.print(output);
	Serial.print("	Speed : ");
	Serial.println(speed);
}

//TODO : Tester auto pilot
void autoControl() {
	double errorRange = 1;	
	double movementRange = 45; //Change to increase the angle

	if (goingUp)
		goal = movementRange;
	else
		goal = -movementRange;

	pidControl();

	double error = goal - input;

	if (abs(error) <= errorRange){
		goingUp = false;
		delay(1000);
	}

}

//TODO : Tester shut down
//Put back the arm to the 0 angle
void shutDown() {
	Serial.println("Moving to 0");
	goal = -position;

	double errorRange = 1;
	double error = goal - input;
	
	while (abs(error) >= errorRange)
		pidControl();
	goal = 0;
	Serial.println("Safe to shut down");
}

#pragma endregion//}

#pragma region "Encoder" //{

//Fonction qui retourne l'angle du moteur, If fullTurn = true, will convert the angle to a circle (0-360), false for no correction
double readAngle(Encoder& encoder) {
	long count;
	double angle;
	count = encoder.read() / encoderResolution;    //To count
	angle = double(count) / countsPerTurn / gearReduction * 360; //To get degrees

	return angle;
}

#pragma endregion //}

#pragma region "Serial Communication" //{
//Read data from the serial port. Return the value if its a number. start with # to call a function, =x to set angle to x
void readSerial(double& angleToGo, Encoder& encoder, Servo& servo) {

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
			executeCommande(commande, encoder, servo);
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

//Choose the commande to execute, #reset reset les angles; #arm arms the esc, #setkp, #setkd ou #setki to change one of those parameters
void executeCommande(String commande, Encoder& encoder, Servo& servo) {

	if (commande == "reset")
		resetAngle(encoder);

	if (commande == "stop")
		goal = 0;

	if (commande == "arm")
		armEsc(servo);

	if (commande == "autopilot") {
		autoPilot = !autoPilot;
		if (autoPilot) 
			Serial.println("Auto Pilot on");
		else
			Serial.println("Auto Pilot off");
	}

	if (commande == "shutdown")
		shutDown();

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


//Commandes pouvant être exécuté

//Reset position if #reset is sent to the serial monitor
void resetAngle(Encoder& encoder) {

	Serial.println("Angle reset to 0");
	encoder.write(0);

}

//Arm the esc if #arm is sent to the serial monitor
void armEsc(Servo& moteur) {
	//int delayToArm = 2000; //set to 2000 when plugged

	Serial.println("Wait...");
	delay(1000);

	Serial.println("Arming... ");
	moteur.write(0);

	delay(2000);

	Serial.println("Esc armed, ready to start");
	armed = true;
}

#pragma endregion //}

#pragma endregion //}

