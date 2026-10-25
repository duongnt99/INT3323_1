const int btnPin = 12;
const int ledGreen = 14;
const int ledYellow = 27;
const int ledRed = 26;
const int motorPin = 25;
const int buzzerPin = 33;

int currentState = 0;
int lastBtnState = HIGH;
int btnState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long lastBlinkTime = 0;
bool blinkState = false;
unsigned long prepStartTime = 0;

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(motorPin, HIGH);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  int reading = digitalRead(btnPin);
  
  if (reading != lastBtnState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != btnState) {
      btnState = reading;
      
      if (btnState == LOW) {
        if (currentState == 0) {
          currentState = 1;
          digitalWrite(ledGreen, LOW);
          digitalWrite(motorPin, LOW);
        } else if (currentState == 1) {
          currentState = 2;
          digitalWrite(ledRed, LOW);
          digitalWrite(buzzerPin, LOW);
          prepStartTime = millis();
        } else if (currentState == 2) {
          currentState = 1;
          digitalWrite(ledYellow, LOW);
        }
      }
    }
  }
  lastBtnState = reading;

  if (currentState == 1) {
    if (millis() - lastBlinkTime >= 500) {
      lastBlinkTime = millis();
      blinkState = !blinkState;
      digitalWrite(ledRed, blinkState);
      digitalWrite(buzzerPin, blinkState);
    }
  } 
  else if (currentState == 2) {
    if (millis() - lastBlinkTime >= 250) {
      lastBlinkTime = millis();
      blinkState = !blinkState;
      digitalWrite(ledYellow, blinkState);
    }
    
    if (millis() - prepStartTime >= 3000) {
      currentState = 0;
      digitalWrite(ledYellow, LOW);
      digitalWrite(ledGreen, HIGH);
      digitalWrite(motorPin, HIGH);
    }
  }
}