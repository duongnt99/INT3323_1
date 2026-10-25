const int aoPin = 34;
const int doPin = 35;
const int ledGreen = 14;
const int ledYellow = 27;
const int ledRed = 26;
const int buzzerPin = 33;

int wetValue = 1000;
int dryValue = 4000;

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
}

void loop() {
  int adcValue = analogRead(aoPin);
  int doValue = digitalRead(doPin);
  
  Serial.print("ADC Value: ");
  Serial.print(adcValue);
  Serial.print(" | DO State: ");
  Serial.println(doValue);
  
  int dryness = map(adcValue, wetValue, dryValue, 0, 100);
  dryness = constrain(dryness, 0, 100);
  
  Serial.print("Dryness: ");
  Serial.print(dryness);
  Serial.println("%");
  
  if (dryness < 35) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);
  } 
  else if (dryness >= 35 && dryness < 70) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);
  } 
  else if (dryness >= 70) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
    digitalWrite(buzzerPin, HIGH);
  }
  
  Serial.println("--------------------");
  delay(1000);
}