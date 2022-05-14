/*
 * Code pour lire une elctrode sur le bicep.
 * Met comPin a HIGH lors de la flexion et LOW lors de l'extension
 * Code de Charles
 */
 
int bicepValue = 0;

int upCount = 0;
int downCount = 0;
int upThres = 50;
int downThres = 50;

int thres = 200;
int elecPin = 15;
int comPin = 18;

bool goingUp = false;

void setup() {
  pinMode(13, OUTPUT);
  digitalWriteFast(13, HIGH);
  Serial.begin(9600);
}

void loop() {
  bicepValue = analogRead(elecPin);

  if (bicepValue >= thres)
     upCount++;
  else if (bicepValue < thres)
     downCount++;
  if(upCount >= upThres){
    upCount = 0;
    downCount = 0;
    goingUp = true;
  }
  else if(downCount >= downThres){
    upCount = 0;
    downCount = 0;
    goingUp =false;  
  }
  
  digitalWrite(comPin, goingUp);
  Serial.println(goingUp);

  delay(10);
   
}
