#include "SystemStates.h"
#include "Application.h"
#include "Arduino.h"

Watch::Watch() : Warning(500, 1000, Application::YELLOW_LED, false, true)
{ }

HotDry::HotDry() : Warning(200, 800, Application::RED_LED, true, false)
{ }

MQAlert::MQAlert() : Warning(200, 400, Application::RED_LED, true, true)
{ }

Warning::Warning(int _warningDuration, int _warningDelay, int _ledPin, bool _buzzerWarn, bool _ledFlash)
{
  warningDuration = _warningDuration;
  warningDelay = _warningDelay;
  ledPin = _ledPin;
  buzzerWarn = _buzzerWarn;
  ledFlash = _ledFlash;
}

void Warning::enterState()
{
  application.setLed(ledPin, true);
  lastWarning = -warningDelay;
  isWarning = false;
}

void Warning::loopState()
{
  unsigned long elapse = millis() - lastWarning;
  if(isWarning && elapse > warningDuration)
  {
    if(ledFlash)
    {
      application.setLed(ledPin, false);
    }
    if(buzzerWarn)
    {
      application.setBuzzer(false);
    }
    isWarning = false;
  }

  if(!isWarning && elapse > warningDelay)
  {
    if(ledFlash)
    {
      application.setLed(ledPin, true);
    }
    if(buzzerWarn)
    {
      application.setBuzzer(true);
    }
    isWarning = true;
    lastWarning = millis();
  }
}

void Warning::exitState()
{
  application.setLed(ledPin, false);
  application.setBuzzer(false);
}