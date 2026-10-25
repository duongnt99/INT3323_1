const int aoPin = 34;
const int doPin = 35;
const int ledGreen = 14;
const int ledYellow = 27;
const int ledRed = 26;
const int buzzerPin = 33;

int baseline = 0;

void setup() {
  Serial.begin(115200);

  pinMode(aoPin, INPUT);
  pinMode(doPin, INPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(buzzerPin, LOW);

  for (int i = 0; i < 10; i++) {
    delay(1000);
  }

  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(aoPin);
    delay(100);
  }
  baseline = sum / 10;
}

void loop() {
  int adcValue = analogRead(aoPin);
  int doValue = digitalRead(doPin);

  Serial.print("AO Value: ");
  Serial.print(adcValue);
  Serial.print(" | DO State: ");
  Serial.print(doValue);
  Serial.print(" | Baseline: ");
  Serial.println(baseline);

  int difference = adcValue - baseline;

  if (difference < 150) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);
  } 
  else if (difference >= 150 && difference < 400) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);
  } 
  else if (difference >= 400) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
    digitalWrite(buzzerPin, HIGH);
  }

  delay(1000);
}