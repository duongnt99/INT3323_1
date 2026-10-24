#include "SystemStates.h"
#include "Components.h"
#include "Arduino.h"

void Initiating::enterState()
{
  flashOn = false;
  previousFlash = -FLASH_DELAY;
  warningTimeout = millis() + WARNING_DURATION;
}

void Initiating::loopState()
{
  handleButton();
  handleWarning();
}

void Initiating::handleButton()
{
  if(isButtonPressed() && millis() < warningTimeout)
  {
    switchState(StateIndex::Stopped);
  }
}

void Initiating::handleWarning()
{
  unsigned long time = millis();
  if(time > warningTimeout)
  {
    switchState(StateIndex::Ready);
    return;
  }

  unsigned long elapse = time - previousFlash;
  if(flashOn && elapse > FLASH_DURATION)
  {
    setLed(YELLOW_LED, false);
    flashOn = false;
  }

  if(!flashOn && elapse > FLASH_DELAY)
  {
    setLed(YELLOW_LED, true);
    flashOn = true;
    previousFlash = millis();
  }
}

void Initiating::exitState()
{
  setLed(YELLOW_LED, false);
}