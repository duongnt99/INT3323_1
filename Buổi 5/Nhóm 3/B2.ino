#include <Arduino.h>

const int GREEN_LED  = 25;
const int YELLOW_LED = 26;
const int RED_LED    = 27;
const int BUZZER     = 13;

const int SOIL_AO = 34; 
const int SOIL_DO = 35; 

const int BUZZER_ON  = LOW;
const int BUZZER_OFF = HIGH;


const int WET_VALUE = 1200; 
const int DRY_VALUE = 3500; 

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

  pinMode(SOIL_DO, INPUT);

  digitalWrite(BUZZER, BUZZER_OFF);
  setLeds(false, false, false);
}

void loop() {
  int rawADC = analogRead(SOIL_AO);
  int digitalVal = digitalRead(SOIL_DO);

  int dryness = map(rawADC, WET_VALUE, DRY_VALUE, 0, 100);
  dryness = constrain(dryness, 0, 100);

  String status = "";

  if (dryness < 35) {
    status = "WET";
    setLeds(false, false, true);
    digitalWrite(BUZZER, BUZZER_OFF);
  } else if (dryness < 70) {
    status = "MODERATE";
    setLeds(false, true, false);
    digitalWrite(BUZZER, BUZZER_OFF);
  } else {
    status = "DRY";
    setLeds(true, false, false);
    digitalWrite(BUZZER, BUZZER_ON);
  }

  Serial.print("ADC: ");
  Serial.print(rawADC);
  Serial.print(" | DO: ");
  Serial.print(digitalVal);
  Serial.print(" | Do kho: ");
  Serial.print(dryness);
  Serial.print("% | Trang thai: ");
  Serial.println(status);

  delay(1000);
}