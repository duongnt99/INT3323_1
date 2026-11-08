#include "esp32-hal-adc.h"
#include <IoTNode.h>
#include <Arduino.h>
#include <DHT.h>

IoTNode iotNode;

IoTNode::IoTNode() : dht(DHT_PIN, DHT11)
{ }

void IoTNode::setUp()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(MQ2_DIGITAL, INPUT);
  setLed(true);

  analogReadResolution(12);

  dht.begin();
}

void IoTNode::setLed(bool on)
{
  digitalWrite(LED_PIN, on ? HIGH : LOW);
}

Telemetry IoTNode::getTelemetry()
{
  Telemetry telemetry;

  telemetry.temperature = dht.readTemperature();
  telemetry.humidity = dht.readHumidity();
  telemetry.trustedTemperatureHumidity = !(isnan(telemetry.temperature) || isnan(telemetry.humidity));

  telemetry.soilAnalog = analogRead(SOIL_ANALOG);
  telemetry.gasAnalog = analogRead(MQ2_ANALOG);
  telemetry.gasDigital = digitalRead(MQ2_DIGITAL);

  return telemetry;
}