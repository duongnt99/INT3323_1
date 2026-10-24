#include "SystemStates.h"
#include "Components.h"

void Standby::enterState()
{
  setLed(RED_LED, true);
  setMotor(false);
}

void Standby::loopState()
{
  if(!isButtonPressed())
  {
    return;
  }

  switchState(StateIndex::Running);
}

void Standby::exitState()
{
  setLed(RED_LED, false);
}