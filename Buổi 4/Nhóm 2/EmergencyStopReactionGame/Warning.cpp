#include "SystemStates.h"
#include "Components.h"
#include "Arduino.h"

void Warning::enterState()
{
  warningStart = millis();
  flashStart = -FLASH_DELAY;
  isFlashing = false;
}

void Warning::loopState()
{
  handleFlash();
  handleButton();
}

void Warning::handleFlash()
{
  unsigned long elapse = millis() - flashStart;
  if(isFlashing && elapse > FLASH_DURATION)
  {
    setLed(YELLOW_LED, false);
    setBuzzer(false);

    isFlashing = false;
  }

  if(!isFlashing && elapse > FLASH_DELAY)
  {
    setLed(YELLOW_LED, true);
    setBuzzer(true);

    isFlashing = true;
    flashStart = millis();
  }
}

void Warning::handleButton()
{
  if(!isButtonPressed())
  {
    return;
  }

  unsigned long elapse = millis() - warningStart;
  Serial.printf("Button pressed in %.3fs.\n", elapse / 1000.0f);
  bool succeeded = elapse < REACTION_TIMEOUT;
  switchState(succeeded ? StateIndex::Succeeded : StateIndex::Failed);
}

void Warning::exitState()
{
  setLed(YELLOW_LED, false);
  setBuzzer(false);
}