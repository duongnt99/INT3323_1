#include <Arduino.h>

const int BUTTON     = 14;
const int GREEN_LED  = 32;
const int YELLOW_LED = 33;
const int RED_LED    = 27;
const int BUZZER     = 13;

const int ENA = 25; 
const int IN1 = 26; 

const int MOTOR_SPEED = 255;
const unsigned long MOTOR_RUN_TIME = 5000; // Sửa lại thành 3s hoặc 10s cho phần mở rộng

bool isRunning = false;

bool checkButtonPressed() {
  static int lastState = HIGH;
  static int stableState = HIGH;
  static unsigned long lastDebounceTime = 0;

  int reading = digitalRead(BUTTON);
  if (reading != lastState) {
    lastDebounceTime = millis();
  }
  lastState = reading;

  if ((millis() - lastDebounceTime) >= 40) {
    if (reading != stableState) {
      stableState = reading;
      return (stableState == LOW); 
    }
  }
  return false;
}

void setReadyState() {
  analogWrite(ENA, 0);          
  digitalWrite(RED_LED, HIGH);   
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, HIGH);
}

void chayBangChuyen() {
  isRunning = true; 

  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);

  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, LOW);
    delay(150);                
    digitalWrite(BUZZER, HIGH);
    delay(850);                
  }
  digitalWrite(YELLOW_LED, LOW);

  digitalWrite(GREEN_LED, HIGH);
  analogWrite(ENA, MOTOR_SPEED);
  delay(MOTOR_RUN_TIME);       

  analogWrite(ENA, 0);        
  digitalWrite(GREEN_LED, LOW);

  for (int i = 0; i < 3; i++) {
    delay(200);
    digitalWrite(GREEN_LED, HIGH);
    delay(200);
    digitalWrite(GREEN_LED, LOW);
  }

  setReadyState();
  isRunning = false;           
}

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, HIGH);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);

  digitalWrite(IN1, HIGH);     
  setReadyState();             
}

void loop() {
  if (!isRunning && checkButtonPressed()) {
    chayBangChuyen();
  }
}