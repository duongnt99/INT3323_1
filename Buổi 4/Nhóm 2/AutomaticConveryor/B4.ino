const int RED_LED_PIN = 17;
const int GREEN_LED_PIN = 23;
const int YELLOW_LED_PIN = 22;
const int BUZZER_PIN = 16;
const int MOTOR_PIN = 25;
const int MOTOR_AUX_PIN = 26;
const int BUTTON_PIN = 13;

const int DEBOUNCE_DELAY = 50;

const int BUZZER_DURATION = 150;
const int BUZZER_DELAY = 1000;

const int LED_FLASH_DURATION = 300;
const int LED_FLASH_DELAY = 600;
const int LED_FLASH_COUNT = 3;
const int FINISHED_DELAY_ENTRY = 500;

const int COUNTDOWN_DURATION = 3000;
const int OPERATING_DURATION = 5000;

const uint8_t MOTOR_POWER = 150;

enum State
{
  Ready,
  Countdown,
  Operating,
  Finished
};

State currentState = Ready;

int lastButtonState = HIGH;
int buttonState = HIGH;
unsigned long lastDebounceTime = 0;

unsigned long countdownStart = 0;
unsigned long buzzerStart = 0;
bool buzzerOn = false;

unsigned long finishedStart = 0;
unsigned long ledFlashStart = 0;
bool ledOn = false;
int remainingLedFlash = 0;

unsigned long operatingStart = 0;

void setup() 
{
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_AUX_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  startReady();
}

bool isButtonPressed()
{
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  lastButtonState = reading;

  if (millis() - lastDebounceTime > DEBOUNCE_DELAY && reading != buttonState) {
    buttonState = reading;
    return buttonState == LOW;
  }

  return false;
}

void loop() 
{
  switch(currentState)
  {
    case Ready:
      loopReady();
      break;

    case Countdown:
      loopCountdown();
      break;

    case Operating:
      loopOperating();
      break;

    case Finished:
      loopFinished();
      break;
  }
}

void startReady()
{
  digitalWrite(RED_LED_PIN, HIGH);

  currentState = Ready;
}

void startCountdown()
{
  digitalWrite(YELLOW_LED_PIN, HIGH);
  toneBuzzer();

  countdownStart = millis();

  currentState = Countdown;
}

void startOperating()
{
  digitalWrite(GREEN_LED_PIN, HIGH);
  analogWrite(MOTOR_PIN, MOTOR_POWER);

  operatingStart = millis();

  currentState = Operating;
}

void startFinished()
{
  finishedStart = millis();
  ledFlashStart = -LED_FLASH_DELAY;
  remainingLedFlash = LED_FLASH_COUNT;

  digitalWrite(GREEN_LED_PIN, LOW);
  ledOn = false;

  currentState = Finished;
}

void loopReady()
{
  if(!isButtonPressed())
  {
    return;
  }

  digitalWrite(RED_LED_PIN, LOW);
  startCountdown();
}

void loopCountdown()
{
  unsigned long countdownElapse = millis() - countdownStart;
  if(countdownElapse > COUNTDOWN_DURATION)
  {
    digitalWrite(YELLOW_LED_PIN, LOW);
    noTone(BUZZER_PIN);
    startOperating();
    return;
  }

  unsigned long buzzerElapse = millis() - buzzerStart;
  if(buzzerOn && buzzerElapse > BUZZER_DURATION)
  {
    noTone(BUZZER_PIN);
    buzzerOn = false;
  }

  if(!buzzerOn && buzzerElapse > BUZZER_DELAY)
  {
    toneBuzzer();
  }
}

void loopOperating()
{
  if(millis() - operatingStart > OPERATING_DURATION)
  {
    digitalWrite(GREEN_LED_PIN, LOW);
    analogWrite(MOTOR_PIN, 0);
    startFinished();
  }
}

void loopFinished()
{
  unsigned long elapse = millis() - finishedStart;
  if(elapse < FINISHED_DELAY_ENTRY)
  {
    return;
  }

  unsigned long ledElapse = millis() - ledFlashStart;
  if(ledOn && ledElapse > LED_FLASH_DURATION)
  {
    digitalWrite(GREEN_LED_PIN, LOW);
    ledOn = false;
  }

  if(!ledOn && ledElapse > LED_FLASH_DELAY)
  {
    if(remainingLedFlash-- > 0)
    {
      digitalWrite(GREEN_LED_PIN, HIGH);
      ledFlashStart = millis();
      ledOn = true;
    } 
    else
    {
      digitalWrite(GREEN_LED_PIN, LOW);
      ledOn = false;
      startReady();
    }
  }
}

void toneBuzzer()
{
  tone(BUZZER_PIN, 1000);
  buzzerOn = true;
  buzzerStart = millis();
}