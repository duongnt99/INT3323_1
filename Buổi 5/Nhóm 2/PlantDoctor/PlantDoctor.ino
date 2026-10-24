#include "DHT.h"

const int DHT_PIN = 23;
const int SOIL_ANALOG = 35;
const int SOIL_DIGITAL = 25;
const int RED_LED = 16;
const int YELLOW_LED = 17;
const int GREEN_LED = 18;
const int BUZZER = 13;

const int DRY_THRESHOLD = 4095;
const int WET_THRESHOLD = 2000;

DHT dht = DHT(DHT_PIN, DHT11);

void setup()
{
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SOIL_DIGITAL, INPUT);
  analogReadResolution(12);
  dht.begin();
}

void loop()
{
  delay(1000);

  // --- TEMPERATURE AND HUMIDITY

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if(isnan(temperature) || isnan(humidity))
  {
    Serial.println("SENSOR ERROR!");
    setLed(-1);
    setBuzzer(false);
    return;
  }

  Serial.println("--------------------------------------------");
  Serial.printf("Temperature: %.1f C | Humidity: %.2f%%\n", temperature, humidity);

  // --- SOIL MOISTURE

  int analog = analogRead(SOIL_ANALOG);
  int digital = digitalRead(SOIL_DIGITAL);

  int dryness = map(analog, WET_THRESHOLD, DRY_THRESHOLD, 0, 100);
  dryness = constrain(dryness, 0, 100);

  Serial.printf("Analog moisture: %d | Digital state: %d | Wetness: %d%%\n", analog, digital, 100 - dryness);

  // --- CONDITION
  if(dryness >= 70 && temperature >= 30)
  {
    Serial.println("URGENT");
    setLed(RED_LED);
    setBuzzer(true);
  }
  else if(dryness < 55 && temperature < 30 && humidity > 40)
  {
    Serial.println("NORMAL");
    setLed(GREEN_LED);
    setBuzzer(false);
  }
  else
  {
    Serial.println("NEEDS ATTENTION:");
    if(dryness >= 55)
    {
      Serial.printf("- Soil is dry: Dryness at %d%% which exceeds threshold %d%%\n", dryness, 55);
    }

    if(temperature >= 30)
    {
      Serial.printf("- Temperature is high: At %.1f C which exceeds threshold %d C\n", temperature, 30);
    }

    if(humidity < 40)
    {
      Serial.printf("- Air humidity is low: At %.2f%% which is lower than baseline threshold %d%%\n", humidity, 40);
    }

    setLed(YELLOW_LED);
    setBuzzer(false);
  }
}

void setLed(int pin)
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  if(pin > 0)
  {
    digitalWrite(pin, HIGH);
  }
}

void setBuzzer(bool on)
{
  if(on)
  {
    tone(BUZZER, 1000);
  }
  else
  {
    noTone(BUZZER);
  }
}