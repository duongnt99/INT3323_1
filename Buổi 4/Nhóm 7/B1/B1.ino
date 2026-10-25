const int IN1 = 25;
const int IN2 = 26;

void stopMotor()
{
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  stopMotor();
}

void loop() {
  analogWrite(IN1, 255);
  delay(4000);

  analogWrite(IN1, 125);
  delay(4000);

  analogWrite(IN1, 64);
  delay(4000);

  stopMotor();
  delay(3000);
}
