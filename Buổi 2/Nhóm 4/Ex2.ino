#define LED1 23
#define LED2 22
#define LED3 21

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED1, HIGH);
    delay(1000);
    digitalWrite(LED1, LOW);

    digitalWrite(LED2, HIGH);
    delay(1000);
    digitalWrite(LED2, LOW);

    digitalWrite(LED3, HIGH);
    delay(1000);
    digitalWrite(LED3, LOW);

    digitalWrite(LED2, HIGH);
    delay(1000);
    digitalWrite(LED2, LOW);

    digitalWrite(LED1, HIGH);
    delay(1000);
    digitalWrite(LED1, LOW);

    digitalWrite(LED2, HIGH);
    delay(1000);
    digitalWrite(LED2, LOW);

    digitalWrite(LED3, HIGH);
    delay(1000);
    digitalWrite(LED3, LOW);
  }

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED1, HIGH);
    delay(1000);
    digitalWrite(LED1, LOW);

    digitalWrite(LED2, HIGH);
    delay(1000);
    digitalWrite(LED2, LOW);

    digitalWrite(LED3, HIGH);
    delay(1000);
    digitalWrite(LED3, LOW);
  }
}