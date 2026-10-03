#define LED 23
#define BUZZER 22

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(1000);

    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    delay(1000);
  }
}