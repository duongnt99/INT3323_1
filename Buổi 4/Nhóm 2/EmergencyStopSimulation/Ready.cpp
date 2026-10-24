#include "SystemStates.h"
#include "Components.h"

void Ready::enterState()
{
  setMotor(true);
  setLed(GREEN_LED, true);
}

void Ready::loopState()
{
  if(!isButtonPressed())
  {
    return;
  }

  setMotor(false);
  setLed(GREEN_LED, false);

  switchState(StateIndex::Stopped);
}

void Ready::exitState()
{
  setMotor(false);
  setLed(GREEN_LED, false);
}