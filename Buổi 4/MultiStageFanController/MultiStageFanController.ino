const int RED_PIN = 17;  
const int GREEN_PIN = 23;
const int YELLOW_PIN = 22;
const int MOTOR_PIN = 25;
const int MOTOR_AUX_PIN = 26;
const int BUTTON_PIN = 13;
const int BUZZER_PIN = 16;
const int TOTAL_MODE = 4;

const int DEBOUNCE_DELAY = 50;

const int BUZZER_SHORT_DURATION = 150;
const int BUZZER_LONG_DURATION = 700;
const int BUZZER_DELAY = 350;

int lastButtonState = HIGH;
int buttonState;
int currentModeIndex = 0;
unsigned long lastDebounceTime = 0;

bool buzzerOn = false;
int buzzerRemain = 0;
unsigned long buzzerDuration = 0;
unsigned long buzzerOnTime = 0;

struct ModeConfig
{
  int ledPin;
  int buzzerCount;
  uint8_t motorLevel;
  unsigned long buzzerDuration;
} none, first, second, third;

ModeConfig sequences[TOTAL_MODE];

void setup() {
  Serial.begin(115200);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_AUX_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(MOTOR_AUX_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  setupModeConfigurations();
}

void setupModeConfigurations()
{
  none.ledPin = -1;
  none.motorLevel = 0;
  none.buzzerCount = 1;
  none.buzzerDuration = BUZZER_LONG_DURATION;
  sequences[0] = none;

  first.ledPin = GREEN_PIN;
  first.motorLevel = 50;
  first.buzzerCount = 1;
  first.buzzerDuration = BUZZER_SHORT_DURATION;
  sequences[1] = first;

  second.ledPin = YELLOW_PIN;
  second.motorLevel = 100;
  second.buzzerCount = 2;
  second.buzzerDuration = BUZZER_SHORT_DURATION;
  sequences[2] = second;

  third.ledPin = RED_PIN;
  third.motorLevel = 180;
  third.buzzerCount = 3;
  third.buzzerDuration = BUZZER_SHORT_DURATION;
  sequences[3] = third;
}

void loop() {
  handleButton();
  handleBuzzer();
}

void handleButton()
{
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        currentModeIndex = (currentModeIndex + 1) % TOTAL_MODE;
        updateStatus();
      }
    }
  }

  lastButtonState = reading;
}

void updateStatus()
{
  ModeConfig mode = sequences[currentModeIndex];

  turnLedsOff();
  if(mode.ledPin > 0)
  {
    digitalWrite(mode.ledPin, HIGH);
  }

  analogWrite(MOTOR_PIN, mode.motorLevel);

  buzzerRemain = mode.buzzerCount;
  buzzerDuration = mode.buzzerDuration;
  if(mode.buzzerCount > 0)
  {
    toneBuzzer();
  }
}

void turnLedsOff()
{
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
}

void handleBuzzer()
{
  unsigned long buzzerElapse = millis() - buzzerOnTime;
  if(buzzerOn && buzzerElapse > buzzerDuration)
  {
    noTone(BUZZER_PIN);
    buzzerOn = false;
  }

  if(buzzerRemain > 0 && buzzerElapse > BUZZER_DELAY)
  {
    toneBuzzer();
  }
}

void toneBuzzer()
{
  buzzerRemain--;
  buzzerOnTime = millis();

  tone(BUZZER_PIN, 1000);
  buzzerOn = true;
}