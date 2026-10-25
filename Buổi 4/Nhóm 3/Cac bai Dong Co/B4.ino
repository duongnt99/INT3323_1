#include <Arduino.h>

const int BUTTON     = 14;
const int GREEN_LED  = 32;
const int YELLOW_LED = 33;
const int RED_LED    = 27;
const int BUZZER     = 13;

const int ENA = 25; 
const int IN1 = 26; 

const int MOTOR_SPEED = 255;

const int BUZZER_ON  = LOW;
const int BUZZER_OFF = HIGH;

enum SystemState {
  RUNNING,     
  STOPPED,     
  RESTARTING  
};

SystemState currentState = RUNNING;

unsigned long lastBlinkTime = 0;
bool blinkState = false;

unsigned long restartStartTime = 0;
unsigned long lastYellowBlink = 0;
bool yellowState = false;

bool checkButtonPressed() {
  static int lastReading = HIGH;
  static int stableState = HIGH;
  static unsigned long lastDebounceTime = 0;
  bool pressedEvent = false;

  int reading = digitalRead(BUTTON);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  lastReading = reading;

  if ((millis() - lastDebounceTime) >= 40) {
    if (reading != stableState) {
      stableState = reading;
      if (stableState == LOW) {
        pressedEvent = true;
      }
    }
  }
  return pressedEvent;
}

void enterRunningState() {
  currentState = RUNNING;
  analogWrite(ENA, MOTOR_SPEED);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, BUZZER_OFF);
}

void enterStoppedState() {
  currentState = STOPPED;
  analogWrite(ENA, 0); 
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  
  lastBlinkTime = millis();
  blinkState = true;
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BUZZER, BUZZER_ON);
}

void enterRestartingState() {
  currentState = RESTARTING;
  analogWrite(ENA, 0);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, BUZZER_OFF);

  restartStartTime = millis();
  lastYellowBlink = millis();
  yellowState = true;
  digitalWrite(YELLOW_LED, HIGH);
}

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);

  digitalWrite(IN1, HIGH);

  enterRunningState();
}

void loop() {
  bool btnPressed = checkButtonPressed();

  switch (currentState) {
    case RUNNING:
      if (btnPressed) {
        enterStoppedState();
      }
      break;

    case STOPPED:
      if (millis() - lastBlinkTime >= 500) {
        lastBlinkTime = millis();
        blinkState = !blinkState;
        digitalWrite(RED_LED, blinkState ? HIGH : LOW);
        digitalWrite(BUZZER, blinkState ? BUZZER_ON : BUZZER_OFF);
      }

      if (btnPressed) {
        enterRestartingState();
      }
      break;

    case RESTARTING:
      if (btnPressed) {
        enterStoppedState();
        break;
      }

      if (millis() - lastYellowBlink >= 250) {
        lastYellowBlink = millis();
        yellowState = !yellowState;
        digitalWrite(YELLOW_LED, yellowState ? HIGH : LOW);
      }

      if (millis() - restartStartTime >= 3000) {
        enterRunningState();
      }
      break;
  }
}