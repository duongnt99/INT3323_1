#include "Application.h"
#include "Arduino.h"
#include "DHT.h"

Application application;

Application::Application() : dht(DHT_PIN, DHT11), currentState(StateIndex::Normal), nextUpdate(0)
{ }

void Application::setup()
{
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MQ2_DIGITAL, INPUT);

  analogReadResolution(12);

  dht.begin();

  states[StateIndex::Normal] = new Normal();
  states[StateIndex::Watch] = new Watch();
  states[StateIndex::HotDry] = new HotDry();
  states[StateIndex::MQAlert] = new MQAlert();
  states[StateIndex::DhtError] = new DhtError();

  states[currentState]->enterState();
}

void Application::setLed(int pin, bool on)
{
  digitalWrite(pin, on ? HIGH : LOW);
}

void Application::setBuzzer(bool on)
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

void Application::loop()
{
  readSensors();
  loopCurrentState();
}

void Application::switchState(StateIndex state)
{
  if(currentState == state)
  {
    return;
  }

  states[currentState]->exitState();
  currentState = state;
  states[currentState]->enterState();
}

void Application::readSensors()
{
  if(millis() < nextUpdate)
  {
    return;
  }

  nextUpdate = millis() + UPDATE_DELAY;

  Serial.println("-------------------------------------------------------------------------------------------------");

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if(isnan(temperature) || isnan(humidity))
  {
    Serial.println("SENSOR ERROR");
    Serial.println("Error reading temperature and air humidity!");
    switchState(StateIndex::DhtError);
    return;
  }

  int soilAnalog = analogRead(SOIL_ANALOG);
  int gasAnalog = analogRead(MQ2_ANALOG);
  int gasDigital = digitalRead(MQ2_DIGITAL);

  int soilDryness = map(soilAnalog, WET_THRESHOLD, DRY_THRESHOLD, 0, 100);
  soilDryness = constrain(soilDryness, 0, 100);

  Serial.printf(
    "Temperature: %.1f C | Air Humidity: %.2f%% | Soil Dryness: %d%% | Gas Analog: %d | Gas Digital: %d\n",
    temperature,
    humidity,
    soilDryness,
    gasAnalog,
    gasDigital
  );

  int gasDifference = gasAnalog - GAS_BASELINE;
  if(gasDifference > GAS_DIFFERENCE_EXTREME)
  {
    Serial.println("MQ2 ALERT");
    switchState(StateIndex::MQAlert);
    return;
  }
  
  if(temperature >= TEMPERATURE_HOT && soilDryness >= SOIL_DRY_EXTREME)
  {
    Serial.println("HOT & DRY");
    switchState(StateIndex::HotDry);
    return;
  }

  bool badTemperature = temperature >= TEMPERATURE_HOT;
  bool badHumidity = humidity < HUMIDITY_LOW;
  bool badSoilDryness = soilDryness > SOIL_DRY_CAUTIOUS;
  bool badGas = gasDifference > GAS_DIFFERENCE_CAUTIOUS;

  if(!badTemperature && !badHumidity && !badSoilDryness && !badGas)
  {
    Serial.println("NORMAL");
    switchState(StateIndex::Normal);
    return;
  }

  switchState(StateIndex::Watch);
  Serial.println("WATCH:");
  if(badTemperature)
  {
    Serial.printf("- Temperature is high: At %.1f C which exceeds safe threshold %.1f C\n", temperature, TEMPERATURE_HOT);
  }
  if(badHumidity)
  {
    Serial.printf("- Humidity is low: At %.2f%% which is below safe threshold %.2f%%\n", humidity, HUMIDITY_LOW);
  }
  if(badSoilDryness)
  {
    Serial.printf("- Soil is dry: At %d%% dryness which exceeds safe threshold %d%%\n", soilDryness, SOIL_DRY_CAUTIOUS);
  }
  if(badGas)
  {
    Serial.printf("- Gas level is unsafe: At %d difference with baseline %d which exceeds safe threshold %d\n", gasDifference, GAS_BASELINE, GAS_DIFFERENCE_CAUTIOUS);
  }
}
void Application::loopCurrentState()
{
  states[currentState]->loopState();
}