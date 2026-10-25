#include <Arduino.h>
#include <DHT.h>
#define DHT_TYPE DHT11

const int GREEN_LED  = 25;
const int YELLOW_LED = 26;
const int RED_LED    = 27;
const int BUZZER     = 13;

const int SOIL       = 34; 
const int DHT_PIN    = 14;  

const int BUZZER_ON  = LOW;
const int BUZZER_OFF = HIGH;

const int WET_VALUE  = 1500; 
const int DRY_VALUE  = 3500; 

DHT dht(DHT_PIN, DHT_TYPE);

void setLeds(bool red, bool yellow, bool green) {
  digitalWrite(RED_LED, red ? HIGH : LOW);
  digitalWrite(YELLOW_LED, yellow ? HIGH : LOW);
  digitalWrite(GREEN_LED, green ? HIGH : LOW);
}

float calculateDrynessPercentage(int rawADC) {
  float dryness = (float)(rawADC - WET_VALUE) / (DRY_VALUE - WET_VALUE) * 100.0;
  return constrain(dryness, 0.0, 100.0);
}

void setup() {
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, BUZZER_OFF);
  setLeds(false, false, false);

  dht.begin();

  delay(1500); 
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int rawSoil = analogRead(SOIL);
  float soilDryness = calculateDrynessPercentage(rawSoil);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Khong the doc du lieu tu cam bien DHT11!");
    setLeds(true, true, true); 
    digitalWrite(BUZZER, BUZZER_OFF);
    delay(2000);
    return;
  }

  String status = "";
  String reasons = "";

  bool isSoilDryWarning = (soilDryness >= 55.0);
  bool isSoilDryUrgent  = (soilDryness >= 70.0);
  bool isTempHigh       = (temperature >= 30.0);
  bool isHumidityLow    = (humidity < 40.0);

  if (isSoilDryUrgent && isTempHigh) {
    status = "URGENT";
    setLeds(true, false, false);
    digitalWrite(BUZZER, BUZZER_ON);

    reasons = "Dat rat kho (" + String(soilDryness, 1) + "%) Va nhiet do cao (" + String(temperature, 1) + "°C)";
  } 
  else if (isSoilDryWarning || isTempHigh || isHumidityLow) {
    status = "NEEDS ATTENTION";
    setLeds(false, true, false);
    digitalWrite(BUZZER, BUZZER_OFF);

    if (isSoilDryWarning) reasons += "[Dat kho >= 55%] ";
    if (isTempHigh)        reasons += "[Nhiet do >= 30°C] ";
    if (isHumidityLow)     reasons += "[Do am khong khi < 40%] ";
  } 
  else {
    status = "NORMAL";
    setLeds(false, false, true);
    digitalWrite(BUZZER, BUZZER_OFF);
    reasons = "Cac chi so deu chua vuot nguong can chu y( nhiet do < 30, do am dat < 55, do am khong khi < 40)";
  }

  Serial.print("Nhiet do: "); Serial.print(temperature, 1); Serial.print(" °C | ");
  Serial.print("Do am KK: "); Serial.print(humidity, 1); Serial.print(" % | ");
  Serial.print("Do kho dat: "); Serial.print(soilDryness, 1); Serial.println(" %");

  Serial.print("Trang thai : ");
  Serial.println(status);
  Serial.print("Li do   : ");
  Serial.println(reasons);

  delay(2000);
}