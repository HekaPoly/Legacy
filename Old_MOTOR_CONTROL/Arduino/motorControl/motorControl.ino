//Créé le 17 novembre 2017 par Charles
//Modifié le 22/01
//Code pour controller le moteur avec l'encodeur

//TO DO Avant CQI
//Réécrire code nécessaire 
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
double readAngle(Encoder& encoder,bool fullTurn);
void updateAngle(double newAngle, double& angle);
void resetAngle(Encoder& encoder);
void checkIfFullTurn(double& angle, Encoder& encoder);

void readSerial(double& angle, Encoder& encoder, Servo& servo);
void armEsc(Servo& moteur);
void executeCommande(String commande, Encoder& encoder, Servo& servo);

void goToAngle(double angleToGo, Servo& moteur, Encoder& encoder);
int calculateDirection(double angle, double angleToGo);
int calculateSpeed(int percentage, int direction);

//Fonctions à tester :
void turnMotor(int speed, int direction, Servo& moteur);






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
const int turnCW = 1;
const int turnCCW = 2;

//Conraintes sur les angles max et min
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



PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);		//PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, Direction) Option to allow Proportinal on measurment

Encoder encoder1(encoderPin2, encoderPin1);
Servo moteur1;

long positionMoteur1 = -999;
double angleMoteur1 = -999;
bool doOnce = false;
double angleToGo = -1;
double newAngleToGo = -1;
double speed = 0;

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

	//oldController();
	if(!autoPilot)
		pidControl();
	else
		autoControl();
}

void oldController() {
	readSerial(newAngleToGo, encoder1, moteur1);	//Read data from serial monitor, update the angle to go

	if (newAngleToGo >= 0 && newAngleToGo != angleToGo) {	//&& newAngleToGo <= 360 ?
		angleToGo = newAngleToGo;
		goToAngle(angleToGo, moteur1, encoder1);
	}

	//Get position of moteur in degree
	double newAngleMoteur1 = readAngle(encoder1,true);

	//Update the angle and print it if the position changed
	updateAngle(newAngleMoteur1, angleMoteur1);
}

double positionIni = 0;
double position = 0;


//Écrit pour entrer une variation d'angle: -CCW, +CW
void pidControl(){
	
	position = readAngle(encoder1,false);

	//If the goal has changed
	if (goal != oldGoal) {
		Serial.print("New Goal, moving ");
		Serial.print(goal);
		setpoint = goal;
		positionIni = position;
		oldGoal = goal;
	}
	
	//Update input with the Angle
	input = positionIni- position ;

	//Compute 
	myPID.Compute();

	speed = 1500 + output;

	printVariables();

	moteur1.writeMicroseconds(speed);
}

#pragma region "Functions" //{

#pragma region "PID" //{
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

//Vérifier que l'angle est dans le cercle
void checkCircle(double& angle) {
	if (angle > 360)
		angle -= 360;
	else if (angle < 0)
		angle += 360;
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

//TODO : TESTER
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

//TODO : TESTER Put back the arm to the 0 angle
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

//TESTER : Pour old controller, Fonction qui retourne l'angle du moteur, If fullTurn = true, will convert the angle to a circle (0-360), false for no correction
double readAngle(Encoder& encoder, bool fullTurn) {
	long count;
	double angle;
	count = encoder.read() / encoderResolution;    //To count
	angle = double(count) / countsPerTurn / gearReduction * 360; //To get degrees

	if(fullTurn)
		checkIfFullTurn(angle, encoder);

	return angle;
}

//Fonction pour reset la position à chaque tour
//TODO: s'assurer qu'il n'y aille plus d'angle négatif en vérifiant avant de corriger
void checkIfFullTurn(double& angle, Encoder& encoder) {

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

#pragma region "Motor" //{

//Fonction pour faire tourner le moteur jusqu'a un certain angle
void goToAngle(double angleToGo, Servo& moteur, Encoder& encoder) {
	Serial.println("Entering goToAngle");
	double range = 0.1;

	int speedPercentage = 75; //Percentage of max Speed to turn

	double angle = readAngle(encoder,true);

	while (abs(angleToGo - angle) > range) {
		int direction = calculateDirection(angle, angleToGo);
		turnMotor(speedPercentage, direction, moteur);

		//moteur.writeMicroseconds(1500);
		angle = readAngle(encoder,true);
		checkIfFullTurn(angle, encoder1);
		Serial.println(angle);
	}
	moteur.write(pulseMiddle);
	Serial.println("Goal Reached");
	Serial.println(angle);
}

//Speed est le poucentage de la vitesse (0 @ 100); Direction est la direction turnCW ou turnCCW; moteur est le moteur à faire tourner
void turnMotor(int speed, int direction, Servo& moteur) {
	moteur.writeMicroseconds(calculateSpeed(speed, direction));
}

//Calculate the pulse depending of the percentage reveived, return the length of the pulse, 
//TODO: Verifier les nombres weird, ajouter map
int calculateSpeed(int percentage, int direction) {
	int pulseLength = 0;

	if (direction == turnCW) {
		pulseLength = map(percentage, 0, 100, pulseMiddle, pulseMaxCW);
		//pulseLength = 1750;
	}
	else {
		pulseLength = map(percentage, 0, 100, pulseMiddle, pulseMaxCCW);
		//pulseLength = 1250;
	}



	return pulseLength;
}

//Calculate the direction to turn, return 1 for Clock Wise, or 2 for Counter Clock Wise 
//TODO: Verifier wich way to go if both paths are equals
int calculateDirection(double angle, double angleToGo) {
	double d1 = angle - angleToGo;	//Distance 1 between angle and goal
	double d2 = 0;					//Distance 2 between angle and goal

	if (d1 < 0) {
		d1 = -d1;
		d2 = (360 - angleToGo) + angle;

		if (d1 < d2) return turnCCW;
		else return turnCW;
	}

	else {
		d2 = (360 - angle) + angleToGo;

		if (d1 < d2) return turnCW;
		else return turnCCW;
	}

}

#pragma endregion //}

#pragma region "Tests" //{

void testCalculateSpeed() {
	if (!doOnce) {
		Serial.println(calculateSpeed(25, 1));

		Serial.println(calculateSpeed(50, 1));

		Serial.println(calculateSpeed(100, 1));

		Serial.println(calculateSpeed(25, 2));

		Serial.println(calculateSpeed(50, 2));

		Serial.println(calculateSpeed(100, 2));
		doOnce = true;
	}
}

//fonction calculateDirection
void testDirection() {
	//Teste calculate direction, CW = 1, CCW = 2

	if (!doOnce) {

		Serial.println(calculateDirection(90, 270));		//1

		Serial.println(calculateDirection(35, 90));		//2

		Serial.println(calculateDirection(48, 47));		//2

		Serial.println(calculateDirection(350, 5));		//1

		Serial.println(calculateDirection(0, 360));		//??

		Serial.println(calculateDirection(90, 90));		//??

		Serial.println(calculateDirection(360, 360));	//??

		doOnce = true;
	}
}

#pragma endregion //}

#pragma endregion //}

