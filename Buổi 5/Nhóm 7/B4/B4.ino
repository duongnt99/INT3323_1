#include <DHT.h>
#define DHT_PIN 16
#define DHT_TYPE DHT11

const int SOIL_AO = 35;

const int LED_GREEN = 23;
const int LED_YELLOW = 22;
const int LED_RED = 25;
const int BUZZER_PIN = 32;

const int WET_VALUE = 1300;
const int DRY_VALUE = 3200;

DHT dht(DHT_PIN, DHT_TYPE);

void turnOffAll(){
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, HIGH);
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  turnOffAll();

  analogReadResolution(12);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  int rawSoil = analogRead(SOIL_AO);
  int dryness = map(rawSoil, WET_VALUE, DRY_VALUE, 0, 100);
  dryness = constrain(dryness, 0, 100);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("ERROR: Failed to read from DHT11 sensor!");
    turnOffAll();
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C | Humidity: ");
  Serial.print(humidity);
  Serial.print(" % | Soil Dryness: ");
  Serial.print(dryness);
  Serial.println("%");

  // URGENT: Đất rất khô (>=70%) ĐỒNG THỜI nhiệt độ >= 30°C
  if (dryness >= 70 && temperature >= 30.0) {
    turnOffAll();
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println("Status: URGENT");
    Serial.println("Reason: Soil is very dry (>=70%) AND Temperature is high (>=30 C)!");
  } 
  // NEEDS ATTENTION: Đất bắt đầu khô (>=55%) HOẶC nhiệt độ >=30°C HOẶC độ ẩm không khí < 40%
  else if (dryness >= 55 || temperature >= 30.0 || humidity < 40.0) {
    turnOffAll();
    digitalWrite(LED_YELLOW, HIGH);

    Serial.println("Status: NEEDS ATTENTION");
    Serial.print("Reason: ");
    if (dryness >= 55) {
      Serial.print("Soil is getting dry (>=55%) ");
    }
    if (temperature >= 30.0) {
      Serial.print("High temperature (>=30 C) ");
    }
    if (humidity < 40.0) {
      Serial.print("Low air humidity (<40%) ");
    }
    Serial.println();
  } 
  // NORMAL: Tất cả chỉ số nằm trong ngưỡng bình thường
  else {
    turnOffAll();
    digitalWrite(LED_GREEN, HIGH);

    Serial.println("Status: NORMAL");
    Serial.println("Reason: All environmental condition parameters are fine.");
  }

  Serial.println("------------------------------------");

  delay(2000);
}