#include "SystemStates.h"
#include "Application.h"

void Normal::enterState()
{
  application.setLed(Application::GREEN_LED, true);
}

void Normal::loopState()
{ }

void Normal::exitState()
{
  application.setLed(Application::GREEN_LED, false);
}