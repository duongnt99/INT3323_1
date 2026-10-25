#include "DHT.h"

const int DHTPIN = 4;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

const int aoPin = 34;
const int ledGreen = 14;
const int ledYellow = 27;
const int ledRed = 26;
const int buzzerPin = 33;

int wetValue = 1000;
int dryValue = 4000;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(aoPin, INPUT);
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
  delay(2000);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int adcValue = analogRead(aoPin);
  
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT11 SENSOR ERROR!");
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);
    return;
  }
  
  int dryness = map(adcValue, wetValue, dryValue, 0, 100);
  dryness = constrain(dryness, 0, 100);
  
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" C | Hum: ");
  Serial.print(h);
  Serial.print(" % | Soil Dryness: ");
  Serial.print(dryness);
  Serial.println(" %");

  if (dryness >= 70 && t >= 30.0) {
    Serial.println("Status: URGENT - Reason: Soil is very dry AND Temperature is too high!");
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } 
  else if (dryness >= 55 || t >= 30.0 || h < 40.0) {
    Serial.print("Status: NEEDS ATTENTION - Reason: ");
    if (dryness >= 55) {
      Serial.print("Soil starting to dry. ");
    }
    if (t >= 30.0) {
      Serial.print("High temperature. ");
    }
    if (h < 40.0) {
      Serial.print("Low air humidity. ");
    }
    Serial.println();
    
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);
  } 
  else {
    Serial.println("Status: NORMAL - Reason: All conditions are optimal.");
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);
  }
  
  Serial.println("--------------------------------------------------");
}