
#include <DHT.h>

#define DHT_PIN 16
#define DHT_TYPE DHT11

const int SOIL_AO = 35;

const int LED_GREEN  = 25;
const int LED_YELLOW = 26;
const int LED_RED    = 27;
const int BUZZER     = 22;

DHT dht(DHT_PIN, DHT_TYPE);

int wetValue = -1;
int dryValue = -1;
bool calibratedWet = false;
bool calibratedDry = false;

int readAverageSoil() {
  long total = 0;

  for (int i = 0; i < 20; i++) {
    total += analogRead(SOIL_AO);
    delay(20);
  }

  return total / 20;
}

void allOutputsOff() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER, LOW);
}

void setup() {
  Serial.begin(9600);

  dht.begin();
  analogReadResolution(12);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  allOutputsOff();

  Serial.println("Plant Doctor Starting...");
  Serial.println("Put sensor in wet soil, then send w");
  Serial.println("Put sensor in dry soil, then send d");
}

void loop() {
  // Hieu chuan cam bien dat
  if (!calibratedWet || !calibratedDry) {
    if (Serial.available() > 0) {
      char command = Serial.read();

      if (command == 'w' || command == 'W') {
        wetValue = readAverageSoil();
        calibratedWet = true;

        Serial.print("Wet reference: ");
        Serial.println(wetValue);
      }
      else if (command == 'd' || command == 'D') {
        dryValue = readAverageSoil();
        calibratedDry = true;

        Serial.print("Dry reference: ");
        Serial.println(dryValue);
      }

      if (calibratedWet && calibratedDry) {
        if (wetValue == dryValue) {
          Serial.println("Calibration error: values are equal.");
          calibratedWet = false;
          calibratedDry = false;
        } else {
          Serial.println("Calibration completed!");
        }
      }
    }

    allOutputsOff();
    delay(100);
    return;
  }

  // Doc nhiet do va do am khong khi
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Neu DHT11 loi, khong ket luan NORMAL
  if (isnan(temperature) || isnan(humidity)) {
    allOutputsOff();
    Serial.println("SENSOR ERROR: DHT11 read failed!");
    delay(1000);
    return;
  }

  // Doc cam bien dat va tinh do kho tuong doi
  int soilADC = analogRead(SOIL_AO);

  float dryness = 100.0 *
      (soilADC - wetValue) /
      (float)(dryValue - wetValue);

  dryness = constrain(dryness, 0.0, 100.0);

  allOutputsOff();

  Serial.println("\n--- Plant Doctor ---");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Air humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Soil ADC: ");
  Serial.println(soilADC);

  Serial.print("Soil dryness: ");
  Serial.print(dryness, 1);
  Serial.println(" %");

  // URGENT co do uu tien cao nhat
  if (dryness >= 70 && temperature >= 30) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER, HIGH);

    Serial.println("Status: URGENT");
    Serial.println("Reason: Soil very dry AND temperature >= 30 C");
  }
  // NEEDS ATTENTION
  else if (dryness >= 55 ||
           temperature >= 30 ||
           humidity < 40) {
    digitalWrite(LED_YELLOW, HIGH);

    Serial.println("Status: NEEDS ATTENTION");

    if (dryness >= 55) {
      Serial.println("- Soil is getting dry (>= 55%)");
    }

    if (temperature >= 30) {
      Serial.println("- Temperature is high (>= 30 C)");
    }

    if (humidity < 40) {
      Serial.println("- Air humidity is low (< 40%)");
    }
  }
  // NORMAL
  else {
    digitalWrite(LED_GREEN, HIGH);

    Serial.println("Status: NORMAL");
    Serial.println("Reason: All indicators are within limits");
  }

  delay(1000);
}
