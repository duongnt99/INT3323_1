#pragma once

#include "SystemStates.h"
#include <map>

const int RED_LED = 17;
const int YELLOW_LED = 22;
const int GREEN_LED = 23;
const int BUTTON = 13;
const int BUZZER = 16;
const int MOTOR = 25;

const int BUTTON_DEBOUNCE = 50;

void initializeComponents();
void setMotor(bool on);
void setLed(int pin, bool on);
void switchState(StateIndex state);
bool isButtonPressed();
void runLoop();