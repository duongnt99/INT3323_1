#include <Arduino.h>
const int PIN_ENA = 25;  
const int PIN_IN1 = 26;  
const int PIN_IN2 = 27;  

void setup() {
  Serial.begin(115200);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);

}

void loop() {
  Serial.println("PWM 255");
  analogWrite(PIN_ENA, 255);
  delay(4000);

  Serial.println("PWM 220 ");
  analogWrite(PIN_ENA, 220);
  delay(4000);

  Serial.println("PMW 180");
  analogWrite(PIN_ENA, 180);
  delay(4000);

  Serial.println("Dung 3s");
  analogWrite(PIN_ENA, 0);
  delay(3000);

}