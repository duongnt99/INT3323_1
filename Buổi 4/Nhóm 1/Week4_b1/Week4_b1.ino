
const int INA = 25;
const int INB = 26;

void setup() {
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);

  // Chọn một chiều quay
  digitalWrite(INB, LOW);
}

void loop() {
  // PWM = 255 trong 4 giây
  analogWrite(INA, 255);
  delay(4000);

  // PWM = 220 trong 4 giây
  analogWrite(INA, 220);
  delay(4000);

  // PWM = 180 trong 4 giây
  analogWrite(INA, 180);
  delay(4000);

  // Dừng động cơ trong 3 giây
  analogWrite(INA, 0);
  delay(3000);
}
