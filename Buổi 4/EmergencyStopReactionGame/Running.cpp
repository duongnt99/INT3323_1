#include "HardwareSerial.h"
#include "SystemStates.h"
#include "Arduino.h"
#include "Components.h"
#include <cstdlib>

void Running::enterState()
{
  warningTime = millis() + MIN_DELAY + (rand() % (MAX_DELAY - MIN_DELAY));

  setLed(GREEN_LED, true);
  setMotor(true);
}

void Running::loopState()
{
  if(millis() > warningTime)
  {
    switchState(StateIndex::Warning);
  }
  else if(isButtonPressed())
  {
    Serial.println("Button pressed before warning!");
    switchState(StateIndex::Failed);
  }
}

void Running::exitState()
{
  setLed(GREEN_LED, false);
  setMotor(false);
}