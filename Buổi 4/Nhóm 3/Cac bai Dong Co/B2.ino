#include <Arduino.h>

const int LED_RED = 27;
const int LED_GREEN = 33;
const int BUTTON = 14;

const int INA = 25;
const int INB = 26;

bool running = false;

bool buttonClicked() {
  static int lastRaw = HIGH;
  static int stable = HIGH;
  static unsigned long changedAt = 0;

  int raw = digitalRead(BUTTON);

  if (raw != lastRaw) {
    changedAt = millis();
  }

  lastRaw = raw;

  if (millis() - changedAt >= 40 && raw != stable) {
    stable = raw;

    return stable == LOW;
  }

  return false;
}

void motorStop() {
  analogWrite(INA, 0);
  digitalWrite(INB, LOW);
}

void motorRun() {
  digitalWrite(INB, LOW);
  analogWrite(INA, 230);
}

void updateOutputs() {
  if (running) {
    motorRun();

    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  }
  else {
    motorStop();

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  }
}

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);

  Serial.begin(115200);

  updateOutputs();
}

void loop() {
  if (buttonClicked()) {
    running = !running;

    updateOutputs();

    Serial.println(
      running ? "FAN ON" : "FAN OFF"
    );
  }
}