#include <Arduino.h>
#include "DHT.h"

#define DHTTYPE DHT11

const int GREEN_LED  = 25;
const int YELLOW_LED = 26;
const int RED_LED    = 27;
const int DHT_PIN    = 14;  

DHT dht(DHTPIN, DHTTYPE);

void setLeds(bool red, bool yellow, bool green) {
  digitalWrite(RED_LED, red ? HIGH : LOW);
  digitalWrite(YELLOW_LED, yellow ? HIGH : LOW);
  digitalWrite(GREEN_LED, green ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  setLeds(false, false, false);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("SENSOR ERROR");
    setLeds(false, false, false);
    delay(2000);
    return;
  }

  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  String status = "";
  if (temperature >= 31.0) {
    status = "HOT";
    setLeds(true, false, false);
  } else if (temperature < 24.0 || humidity < 40.0 || humidity > 75.0) {
    status = "CAUTION";
    setLeds(false, true, false);
  } else {
    status = "COMFORTABLE";
    setLeds(false, false, true);
  }

  Serial.print("Nhiet do: ");
  Serial.print(temperature, 1);
  Serial.print(" °C | Do am: ");
  Serial.print(humidity, 1);
  Serial.print(" % | Heat Index: ");
  Serial.print(heatIndex, 1);
  Serial.print(" °C | Trang thai: ");
  Serial.println(status);

  delay(2000);
}