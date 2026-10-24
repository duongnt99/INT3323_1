#pragma once

#include "SystemStates.h"
#include <map>
#include "DHT.h"

class Application
{
private:
  static constexpr int BUZZER = 13;
  static constexpr int DHT_PIN = 23;
  static constexpr int SOIL_ANALOG = 35;
  static constexpr int MQ2_ANALOG = 34;
  static constexpr int MQ2_DIGITAL = 19;

  static constexpr int UPDATE_DELAY = 1000;

  static constexpr float TEMPERATURE_HOT = 30;
  static constexpr float HUMIDITY_LOW = 40;
  static constexpr int DRY_THRESHOLD = 4095;
  static constexpr int WET_THRESHOLD = 2000;
  static constexpr int SOIL_DRY_CAUTIOUS = 55;
  static constexpr int SOIL_DRY_EXTREME = 70;
  static constexpr int GAS_BASELINE = 200;
  static constexpr int GAS_DIFFERENCE_CAUTIOUS = 150;
  static constexpr int GAS_DIFFERENCE_EXTREME = 400;

  DHT dht;
  std::map<StateIndex, State*> states;
  StateIndex currentState;
  unsigned long nextUpdate;

  void switchState(StateIndex state);
  void readSensors();
  void loopCurrentState();
public:
  static constexpr int RED_LED = 16;
  static constexpr int YELLOW_LED = 17;
  static constexpr int GREEN_LED = 18;

  Application();
  void setup();
  void loop();
  void setLed(int pin, bool on);
  void setBuzzer(bool on);
};

extern Application application;