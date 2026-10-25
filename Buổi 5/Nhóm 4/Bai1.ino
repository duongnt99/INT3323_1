#include "DHT.h"

const int DHTPIN = 4;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

const int ledRed = 26;
const int ledYellow = 27;
const int ledGreen = 14;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("SENSOR ERROR");
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" *C  |  ");
  Serial.print("Hum: ");
  Serial.print(h);
  Serial.print(" %  |  ");
  Serial.print("Heat Index: ");
  Serial.print(hic);
  Serial.println(" *C");

  if (t >= 31.0) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
  } 
  else if (t < 24.0 || h < 40.0 || h > 75.0) {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen, LOW);
  } 
  else {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, HIGH);
  }
}