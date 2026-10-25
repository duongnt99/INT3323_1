#include <Arduino.h>

const int GREEN_LED  = 25;
const int YELLOW_LED = 26;
const int RED_LED    = 27;
const int BUZZER     = 13;

const int MQ2_AO = 34; 
const int MQ2_DO = 35; 

const int BUZZER_ON  = LOW;
const int BUZZER_OFF = HIGH;

const int THRESHOLD_WARNING = 150;
const int THRESHOLD_ALERT   = 400;

int baselineAO = 0;

void setLeds(bool red, bool yellow, bool green) {
  digitalWrite(RED_LED, red ? HIGH : LOW);
  digitalWrite(YELLOW_LED, yellow ? HIGH : LOW);
  digitalWrite(GREEN_LED, green ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(MQ2_DO, INPUT);

  digitalWrite(BUZZER, BUZZER_OFF);
  setLeds(false, false, false);
  // Chờ cảm biến ổn định  
  for (int i = 0; i < 5; i++) {
    digitalWrite(YELLOW_LED, HIGH);
    delay(500);
    digitalWrite(YELLOW_LED, LOW);
    delay(500);
  }

  baselineAO = analogRead(MQ2_AO);

}

void loop() {
  int rawADC = analogRead(MQ2_AO);
  int digitalVal = digitalRead(MQ2_DO);

  int deltaADC = rawADC - baselineAO;
  if (deltaADC < 0) deltaADC = 0; 

  String status = "";

  if (deltaADC < THRESHOLD_WARNING) {
    status = "NORMAL";
    setLeds(false, false, true);
    digitalWrite(BUZZER, BUZZER_OFF);
  } else if (deltaADC < THRESHOLD_ALERT) {
    status = "WARNING";
    setLeds(false, true, false);
    digitalWrite(BUZZER, BUZZER_OFF);
  } else {
    status = "ALERT";
    setLeds(true, false, false);
    digitalWrite(BUZZER, BUZZER_ON);
  }

  Serial.print("AO: ");
  Serial.print(rawADC);
  Serial.print(" | DO: ");
  Serial.print(digitalVal);
  Serial.print(" | Baseline: ");
  Serial.print(baselineAO);
  Serial.print(" | Delta: +");
  Serial.print(deltaADC);
  Serial.print(" | Trang thai: ");
  Serial.println(status);

  delay(1000);
}