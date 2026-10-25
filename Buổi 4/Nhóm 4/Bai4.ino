const int btnStart = 12;
const int ledRed = 26;
const int ledYellow = 27;
const int ledGreen = 14;
const int motorPin = 25;
const int buzzerPin = 33;

int motorRunTime = 5000;
bool isRunning = false;
int lastBtnState = HIGH;

void setup() {
  pinMode(btnStart, INPUT_PULLUP);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledRed, HIGH);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(motorPin, LOW);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  if (!isRunning) {
    int btnState = digitalRead(btnStart);
    
    if (btnState != lastBtnState) {
      if (btnState == LOW) {
        isRunning = true;
        
        digitalWrite(ledRed, LOW);
        digitalWrite(ledYellow, HIGH);
        
        for (int i = 0; i < 3; i++) {
          digitalWrite(buzzerPin, HIGH);
          delay(500);
          digitalWrite(buzzerPin, LOW);
          delay(500);
        }
        
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledGreen, HIGH);
        digitalWrite(motorPin, HIGH);
        
        delay(motorRunTime);
        
        digitalWrite(motorPin, LOW);
        digitalWrite(ledGreen, LOW);
        
        for (int i = 0; i < 3; i++) {
          digitalWrite(ledGreen, HIGH);
          delay(300);
          digitalWrite(ledGreen, LOW);
          delay(300);
        }
        
        digitalWrite(ledRed, HIGH);
        isRunning = false;
      }
      delay(50);
    }
    lastBtnState = btnState;
  }
}