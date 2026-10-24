#include <stdint.h>
#include "SystemStates.h"
#include "Components.h"
#include "Arduino.h"

uint8_t Succeeded::getLedPin() const
{
  return GREEN_LED;
}

uint8_t Failed::getLedPin() const
{
  return RED_LED;
}

void Result::enterState()
{
  flashStart = -FLASH_DELAY;
  notifyStart = millis();
  isFlashing = false;
}

void Result::loopState()
{
  unsigned long notifyElapse = millis() - notifyStart;
  if(notifyElapse > NOTIFY_DURATION)
  {
    switchState(StateIndex::Standby);
    return;
  }

  unsigned long flashElapse = millis() - flashStart;
  if(isFlashing && flashElapse > FLASH_DURATION)
  {
    setLed(getLedPin(), false);

    isFlashing = false;
  }

  if(!isFlashing && flashElapse > FLASH_DELAY)
  {
    setLed(getLedPin(), true);
    
    isFlashing = true;
    flashStart = millis();
  }
}

void Result::exitState()
{
  setLed(getLedPin(), false);
}