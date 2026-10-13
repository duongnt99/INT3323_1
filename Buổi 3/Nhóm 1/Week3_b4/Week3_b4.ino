int LED1 = 25; //xanh
int LED2 = 26; //vàng
int LED3 = 27; //đỏ
int BUZZER = 33;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  delay(4000);

  for (int i = 0; i < 2; i++) {

    digitalWrite(BUZZER, HIGH);
    delay(250);
    digitalWrite(BUZZER, LOW);
    delay(2750);

  }

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  delay(5000);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  delay(3000);

  for (int i = 0; i < 3; i++) {

    digitalWrite(BUZZER, HIGH);
    delay(250);
    digitalWrite(BUZZER, LOW);
    delay(750);

  }
}