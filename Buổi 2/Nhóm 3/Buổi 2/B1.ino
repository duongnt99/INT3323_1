int LED1 = 33;
int LED2 = 34;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {

  digitalWrite(LED2, LOW);
  digitalWrite(LED1, HIGH);
  delay(1000);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(1000);
}