
const int INA = 25;
const int INB = 26;

void setup() {
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);


  digitalWrite(INB, LOW);
}

void loop() {

  analogWrite(INA, 255);
  delay(4000);


  analogWrite(INA, 220);
  delay(4000);


  analogWrite(INA, 180);
  delay(4000);


  analogWrite(INA, 0);
  delay(3000);
}
