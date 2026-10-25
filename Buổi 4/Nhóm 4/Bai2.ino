const int btnPin = 12;
const int redPin = 13;
const int greenPin = 14;
const int motorPin = 25;

bool motorState = false;
int lastBtnState = HIGH;

void setup() {
  Serial.begin(115200);
  
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(motorPin, OUTPUT);

  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(motorPin, LOW);
  
  Serial.println("OFF");
}

void loop() {
  int btnState = digitalRead(btnPin);
  
  if (btnState != lastBtnState) {
    if (btnState == LOW) {
      motorState = !motorState;
      
      if (motorState) {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, HIGH);
        digitalWrite(motorPin, HIGH);
        Serial.println("ON");
      } else {
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);
        digitalWrite(motorPin, LOW);
        Serial.println("OFF");
      }
    }
    delay(50);
  }
  
  lastBtnState = btnState;
}