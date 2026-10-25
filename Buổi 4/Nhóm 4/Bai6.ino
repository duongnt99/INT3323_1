const int btnPin = 12;
const int ledRed = 26;
const int ledYellow = 27;
const int ledGreen = 14;
const int motorPin = 25;
const int buzzerPin = 33;

int state = 0;
int lastBtnState = HIGH;
unsigned long stateStartTime = 0;
unsigned long randomDelay = 0;
unsigned long lastBlinkTime = 0;
bool blinkState = false;

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  randomSeed(analogRead(0));
  
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(motorPin, LOW);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  int btnState = digitalRead(btnPin);
  bool btnPressed = false;
  
  if (btnState == LOW && lastBtnState == HIGH) {
    delay(50); 
    if (digitalRead(btnPin) == LOW) {
      btnPressed = true;
    }
  }
  lastBtnState = btnState;

  if (state == 0) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(motorPin, LOW);
    digitalWrite(buzzerPin, LOW);
    
    if (btnPressed) {
      state = 1;
      digitalWrite(ledRed, LOW);
      digitalWrite(ledGreen, HIGH);
      digitalWrite(motorPin, HIGH);
      randomDelay = random(3000, 6001);
      stateStartTime = millis();
    }
  } 
  else if (state == 1) {
    if (btnPressed) {
      state = 4;
      digitalWrite(ledGreen, LOW);
      digitalWrite(motorPin, LOW);
      stateStartTime = millis();
    } else if (millis() - stateStartTime >= randomDelay) {
      state = 2;
      digitalWrite(ledGreen, LOW);
      stateStartTime = millis();
    }
  } 
  else if (state == 2) {
    if (millis() - lastBlinkTime >= 100) {
      blinkState = !blinkState;
      digitalWrite(ledYellow, blinkState);
      digitalWrite(buzzerPin, blinkState);
      lastBlinkTime = millis();
    }
    
    if (btnPressed) {
      state = 3;
      digitalWrite(ledYellow, LOW);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(motorPin, LOW);
      stateStartTime = millis();
    } else if (millis() - stateStartTime >= 2000) {
      state = 4;
      digitalWrite(ledYellow, LOW);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(motorPin, LOW);
      stateStartTime = millis();
    }
  } 
  else if (state == 3) {
    if (millis() - lastBlinkTime >= 250) {
      blinkState = !blinkState;
      digitalWrite(ledGreen, blinkState);
      lastBlinkTime = millis();
    }
    if (millis() - stateStartTime >= 2000) {
      state = 0;
    }
  } 
  else if (state == 4) {
    if (millis() - lastBlinkTime >= 250) {
      blinkState = !blinkState;
      digitalWrite(ledRed, blinkState);
      digitalWrite(buzzerPin, blinkState);
      lastBlinkTime = millis();
    }
    if (millis() - stateStartTime >= 2000) {
      state = 0;
    }
  }
}