const int LED1 = 23;
const int LOA = 22;
void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LOA, OUTPUT);
}

void loop() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LOA, HIGH);
  delay(1000);
  digitalWrite(LOA, LOW);
  delay(1000);
}