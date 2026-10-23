#include "Components.h"
#include "Arduino.h"

std::map<StateIndex, State*> states;
StateIndex currentState;
StateIndex nextState;
bool stateSwitchPending;

void initializeComponents()
{
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  states[StateIndex::Standby] = new Standby();
  states[StateIndex::Running] = new Running();
  states[StateIndex::Warning] = new Warning();
  states[StateIndex::Succeeded] = new Succeeded();
  states[StateIndex::Failed] = new Failed();

  currentState = StateIndex::Standby;
  states[currentState]->enterState();
}

void setMotor(bool on)
{
  analogWrite(MOTOR, on ? 50 : 0);
}

void setLed(int pin, bool on)
{
  digitalWrite(pin, on ? HIGH : LOW);
}

void setBuzzer(bool on)
{
  if(on)
  {
    tone(BUZZER, 1000);
  }
  else{
    noTone(BUZZER);
  }
}

bool isButtonPressed()
{
  static int lastState;
  static int buttonState;
  static unsigned long lastDebounce;

  int reading = digitalRead(BUTTON);
  if(reading != lastState)
  {
    lastDebounce = millis();
  }

  lastState = reading;
  if(millis() - lastDebounce > BUTTON_DEBOUNCE && reading != buttonState)
  {
    buttonState = reading;
    return buttonState == LOW;
  }

  return false;
}

void switchState(StateIndex state)
{
  if(nextState == state)
  {
    return;
  }

  nextState = state;
  stateSwitchPending = true;
}

void runLoop()
{
  if(stateSwitchPending)
  {
    states[currentState]->exitState();
    currentState = nextState;
    states[currentState]->enterState();
    stateSwitchPending = false;
  }

  states[currentState]->loopState();
}