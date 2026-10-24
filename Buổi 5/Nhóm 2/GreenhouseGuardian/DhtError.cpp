#include "SystemStates.h"
#include "Application.h"

void DhtError::enterState()
{
  application.setLed(Application::RED_LED, false);
  application.setLed(Application::YELLOW_LED, false);
  application.setLed(Application::GREEN_LED, false);
}

void DhtError::loopState()
{ }

void DhtError::exitState()
{ }