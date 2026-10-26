const int LED    = 23;
const int BUZZER = 22;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(1000);

  digitalWrite(LED, LOW);
  digitalWrite(BUZZER, LOW);
  delay(1000);
}