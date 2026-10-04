#include <Arduino.h>

#define BUZZER_PIN 25
#define LED1 26
#define LED2 27
#define LED3 33

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void loop() {
  digitalWrite(LED1, HIGH);
  delay(2000);
  digitalWrite(LED1, LOW);

  int a = 400;
  for(int i = 0; i < 5; i++ ) {
    if (i >= 2) {
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED2, HIGH);
      delay(a);
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(LED2, LOW);
      delay(a);
      a -= 100;
    }
    else {
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED2, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(LED2, LOW);
      delay(800);
    }
  }          

  digitalWrite(LED3, HIGH);
  digitalWrite(BUZZER_PIN, LOW); 
  delay(300);                     
  digitalWrite(LED3, LOW);

  for(int i = 0; i < 5; i++ ) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    delay(200);
  }
  digitalWrite(BUZZER_PIN, HIGH);  
  delay(5000);                     
}