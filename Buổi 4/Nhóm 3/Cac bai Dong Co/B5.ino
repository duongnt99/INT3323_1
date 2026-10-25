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
  IDLE,
  RUNNING,
  WARNING,
  SUCCESS,
  FAIL
};

SystemState currentState = IDLE;

unsigned long stateStartTime = 0;
unsigned long randomFaultTime = 0;

unsigned long lastBlinkTime = 0;
bool blinkState = false;

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

void enterIdleState() {
  currentState = IDLE;
  analogWrite(ENA, 0);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, BUZZER_OFF);
}

void enterRunningState() {
  currentState = RUNNING;
  analogWrite(ENA, MOTOR_SPEED);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, BUZZER_OFF);

  stateStartTime = millis();
  randomFaultTime = random(3000, 6001); 
}

void enterWarningState() {
  currentState = WARNING;
  digitalWrite(GREEN_LED, LOW);
  
  stateStartTime = millis();
  lastBlinkTime = millis();
  blinkState = true;
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(BUZZER, BUZZER_ON);
}

void enterSuccessState() {
  currentState = SUCCESS;
  analogWrite(ENA, 0);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, BUZZER_OFF);

  stateStartTime = millis();
  lastBlinkTime = millis();
  blinkState = true;
  digitalWrite(GREEN_LED, HIGH);
}

void enterFailState() {
  currentState = FAIL;
  analogWrite(ENA, 0);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  stateStartTime = millis();
  lastBlinkTime = millis();
  blinkState = true;
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BUZZER, BUZZER_ON);
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

  randomSeed(analogRead(34)); 
  enterIdleState();
}

void loop() {
  bool btnPressed = checkButtonPressed();

  switch (currentState) {
    case IDLE:
      if (btnPressed) {
        enterRunningState();
      }
      break;

    case RUNNING:
      if (btnPressed) {
        enterFailState(); 
        break;
      }

      if (millis() - stateStartTime >= randomFaultTime) {
        enterWarningState();
      }
      break;

    case WARNING:
      if (btnPressed) {
        enterSuccessState(); 
        break;
      }

      if (millis() - stateStartTime >= 2000) {
        enterFailState(); 
        break;
      }

      if (millis() - lastBlinkTime >= 150) {
        lastBlinkTime = millis();
        blinkState = !blinkState;
        digitalWrite(YELLOW_LED, blinkState ? HIGH : LOW);
        digitalWrite(BUZZER, blinkState ? BUZZER_ON : BUZZER_OFF);
      }
      break;

    case SUCCESS:
      if (millis() - lastBlinkTime >= 200) {
        lastBlinkTime = millis();
        blinkState = !blinkState;
        digitalWrite(GREEN_LED, blinkState ? HIGH : LOW);
      }

      if (millis() - stateStartTime >= 2000) {
        enterIdleState();
      }
      break;

    case FAIL:
      if (millis() - lastBlinkTime >= 200) {
        lastBlinkTime = millis();
        blinkState = !blinkState;
        digitalWrite(RED_LED, blinkState ? HIGH : LOW);
        digitalWrite(BUZZER, blinkState ? BUZZER_ON : BUZZER_OFF);
      }

      if (millis() - stateStartTime >= 2000) {
        enterIdleState();
      }
      break;
  }
}