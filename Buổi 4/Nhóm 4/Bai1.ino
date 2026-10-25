const int ENA = 25;
const int IN1 = 26;
const int IN2 = 27;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void loop() {
  analogWrite(ENA, 255);
  delay(4000);

  analogWrite(ENA, 220);
  delay(4000);

  analogWrite(ENA, 180);
  delay(4000);

  analogWrite(ENA, 0);
  delay(3000);
}