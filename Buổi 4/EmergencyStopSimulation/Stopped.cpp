#include "SystemStates.h"
#include "Components.h"
#include "Arduino.h"

void Stopped::enterState()
{
  flashOn = false;
  previousFlash = -FLASH_DELAY;
}

void Stopped::loopState()
{
  handleWarning();
  handleButton();
}

void Stopped::handleButton()
{
  if(!isButtonPressed())
  {
    return;
  }

  switchState(StateIndex::Initiating);
}

void Stopped::handleWarning()
{
  unsigned long elapse = millis() - previousFlash;
  if(flashOn && elapse > FLASH_DURATION)
  {
    setLed(RED_LED, false);
    flashOn = false;
  }

  if(!flashOn && elapse > FLASH_DELAY)
  {
    setLed(RED_LED, true);
    flashOn = true;
    previousFlash = millis();
  }
}

void Stopped::exitState()
{
  setLed(RED_LED, false);
}