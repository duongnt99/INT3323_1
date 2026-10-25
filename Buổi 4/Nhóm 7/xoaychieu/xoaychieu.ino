const int IN1 = 25;
const int IN2 = 26;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void stopMotor()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(3000);

  stopMotor();
  delay(500);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(3000);

  stopMotor();
  delay(500);
}
