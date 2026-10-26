// B1: Dieu khien chu trinh PWM dong co DC
const int PIN_ENA = 25; // Chan PWM dieu khien toc do
const int PIN_IN1 = 26; // Chan huong 1
const int PIN_IN2 = 27; // Chan huong 2

void setup() {
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  // Dong co quay co dinh 1 chieu
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
}

void loop() {
  // 1. Chay voi PWM = 255 trong 4 giay
  analogWrite(PIN_ENA, 255);
  delay(4000);

  // 2. Giam PWM xuong 220 trong 4 giay
  analogWrite(PIN_ENA, 220);
  delay(4000);

  // 3. Giam PWM xuong 180 trong 4 giay
  analogWrite(PIN_ENA, 180);
  delay(4000);

  // 4. Dung dong co trong 3 giay
  analogWrite(PIN_ENA, 0);
  delay(3000);
}