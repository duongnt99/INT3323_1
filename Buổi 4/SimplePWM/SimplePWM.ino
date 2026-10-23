const int IN_A = 25;
const int IN_B = 26;

void setup() {
  pinMode(IN_A, OUTPUT);
  pinMode(IN_B, OUTPUT);

  digitalWrite(IN_A, LOW);
  digitalWrite(IN_B, LOW);
}

void loop() {
  analogWrite(IN_A, 255);
  delay(4000);
  analogWrite(IN_A, 200);
  delay(4000);
  analogWrite(IN_A, 180);
  delay(4000);
  analogWrite(IN_A, 0);
  delay(3000);
}
