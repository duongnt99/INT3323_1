const int btnPin = 12;
const int ledGreen = 14;
const int ledYellow = 27;
const int ledRed = 26;
const int motorPin = 25;
const int buzzerPin = 33;

int pwmLevel1 = 128;
int pwmLevel2 = 192;
int pwmLevel3 = 255;

int fanState = 0;
int lastBtnState = HIGH;

void shortBeep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}

void longBeep() {
  digitalWrite(buzzerPin, HIGH);
  delay(600);
  digitalWrite(buzzerPin, LOW);
}

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(buzzerPin, LOW);
  analogWrite(motorPin, 0);
}

void loop() {
  int btnState = digitalRead(btnPin);
  
  if (btnState != lastBtnState) {
    if (btnState == LOW) {
      fanState++;
      if (fanState > 3) {
        fanState = 0;
      }
      
      if (fanState == 1) {
        analogWrite(motorPin, pwmLevel1);
        digitalWrite(ledGreen, HIGH);
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledRed, LOW);
        shortBeep(1);
      } else if (fanState == 2) {
        analogWrite(motorPin, pwmLevel2);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledYellow, HIGH);
        digitalWrite(ledRed, LOW);
        shortBeep(2);
      } else if (fanState == 3) {
        analogWrite(motorPin, pwmLevel3);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledRed, HIGH);
        shortBeep(3);
      } else if (fanState == 0) {
        analogWrite(motorPin, 0);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledRed, LOW);
        longBeep();
      }
    }
    delay(50);
  }
  
  lastBtnState = btnState;
}