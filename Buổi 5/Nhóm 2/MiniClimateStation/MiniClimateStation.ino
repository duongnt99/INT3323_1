#include "DHT.h"

const int DHT_PIN = 23;
const int RED_LED = 16;
const int YELLOW_LED = 17;
const int GREEN_LED = 18;

DHT dht = DHT(DHT_PIN, DHT11);

void setup()
{
  Serial.begin(115200);
  dht.begin();

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
}

void loop()
{
  delay(2000);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if(isnan(temperature) || isnan(humidity))
  {
    Serial.println("SENSOR ERROR!");
    setLed(-1);
    return;
  }

  Serial.println("--------------------------------------------");
  Serial.printf("Temperature: %.1f C | Humidity: %.2f%%\n", temperature, humidity);

  if(temperature >= 31)
  {
    Serial.println("HOT");
    setLed(RED_LED);
  }
  else if(temperature < 24 || humidity < 40 || humidity > 75)
  {
    Serial.println("CAUTION");
    setLed(YELLOW_LED);
  }
  else
  {
    Serial.println("COMFORTABLE");
    setLed(GREEN_LED);
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