#pragma once

#include <DHT.h>

struct Telemetry
{
  float temperature;
  float humidity;
  bool trustedTemperatureHumidity;
  int soilAnalog;
  int gasAnalog;
  int gasDigital;
};

class IoTNode
{
private:
  static constexpr int LED_PIN = 23;
  static constexpr int DHT_PIN = 26;
  static constexpr int SOIL_ANALOG = 35;
  static constexpr int MQ2_ANALOG = 34;
  static constexpr int MQ2_DIGITAL = 19;

  DHT dht;
public:
  IoTNode();
  void setUp();
  void setLed(bool on);
  Telemetry getTelemetry();
};

extern IoTNode iotNode;