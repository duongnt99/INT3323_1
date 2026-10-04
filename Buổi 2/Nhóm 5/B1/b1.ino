int led1Pin = 22;
int led2Pin = 23;
int delayTime = 1000;
void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
}

void loop() {
  digitalWrite(led1Pin, HIGH);
  digitalWrite(led2Pin, LOW);
  delay(delayTime);
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, HIGH);
  delay(delayTime);
}