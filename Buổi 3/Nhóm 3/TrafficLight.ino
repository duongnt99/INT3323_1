#include <Arduino.h>

#define BUZZER_PIN 25
#define LED1 26
#define LED2 27
#define LED3 33

void setup() {
  digitalWrite(BUZZER_PIN, HIGH);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void loop() {
  digitalWrite(LED3, HIGH);
  delay(4000);
  for(int i = 0; i< 2; i++) {
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2500);
  }
  digitalWrite(LED3,LOW);
  digitalWrite(LED2, HIGH);
  delay(5000);
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, HIGH);
  delay(3000);
  for(int i = 0; i< 3;i++) {
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(800);
  }
  digitalWrite(LED1, LOW);
}