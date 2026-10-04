const int ledPin = 23;
const int buzzerPin = 22;
const int delayTime = 1000;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(delayTime);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(delayTime);
}