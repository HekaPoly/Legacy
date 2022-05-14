/*
 * Code pour lire une elctrode sur le bicep.
 * Met comPin a HIGH lors de la flexion et LOW lors de l'extension
 * Code de Nic
 */

int upcnt = 0;
int downcnt = 0;
int delay_time = 10;
bool up = true;
int upThres = 700;
int downThres = 550;
int comPin = 18;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(comPin, OUTPUT);
  digitalWriteFast(13, HIGH);
}


void loop() {
 
  auto biceps = analogRead(15);
  Serial.print(biceps);
  Serial.print("\t");

  if (up)
  {

    Serial.println("up");
    if (biceps < downThres)
      downcnt++;
    else 
      downcnt = 0;

    if (downcnt > 500 / delay_time)
    {
      up = false;
      
    digitalWrite(comPin, up);
    digitalWrite(13, up);
    }
    
  }
  else
  {
    Serial.println("down");

    if (biceps > upThres)
      upcnt++;
    else 
      upcnt = 0;

    if (upcnt > 500 / delay_time)
    {
      up = true;
      
    digitalWrite(comPin, up);
    digitalWrite(13, up);
    }

  }
  delay(delay_time);

}
