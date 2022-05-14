#include <Wire.h>


void setup() {
  // put your setup code here, to run once:

  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  printMenu()
}

void printMenu() {
  Serial.print("Entrez la commande à effectuer: (sous le format: slave commande)");
  String address;
  String command;
  char c;

  while (Serial.available() && (c = Serial.read()) != ' ') {
		address += c;
		delay(2);
	}

  while(Serial.available()) {
    c = Serial.read();
		command += c;
		delay(2);
  }

  sendCommand(address.toInt(), command);

}

void sendCommand(int slaveAddress, String command) {
  Wire.beginTransmission(slaveAddress);
  for(int i = 0; i < command.length(); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(); 
}

/*void read(int slaveAddress) {
  String messageRead = "";
  Wire.requestFrom(slaveAddress, /*nb of bytes??*/ /*);
  while(Wire.available()) {
    messageRead += Wire.read();
  }
  Serial.print(messageRead);
}*/
