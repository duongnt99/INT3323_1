// Bai 4: He thong dung khan cap gia lap (FSM + millis)
const int PIN_BUTTON    = 18;
const int PIN_LED_RED   = 19;
const int PIN_LED_YEL   = 22;
const int PIN_LED_GREEN = 21;
const int PIN_BUZZER    = 4;
const int PIN_ENA       = 25;
const int PIN_IN1       = 26;
const int PIN_IN2       = 27;

enum SystemState {
  STATE_RUNNING,    // Motor chay, LED xanh sang
  STATE_EMERGENCY,  // Dung khang cap, LED do + buzzer chop canh bao
  STATE_RESTARTING  // Chuan bi khoi dong lai (LED vang nhap nhay 3s)
};

SystemState currentState = STATE_RUNNING;

// Debounce button
int lastButtonReading = HIGH;
int buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Timer millis
unsigned long stateStartTime = 0;
unsigned long lastBlinkTime = 0;
bool blinkFlag = false;

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YEL, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);

  // Khoi dong ban dau: Motor chay, LED xanh sang
  digitalWrite(PIN_LED_GREEN, HIGH);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_YEL, LOW);
  digitalWrite(PIN_BUZZER, LOW);
  analogWrite(PIN_ENA, 255);
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Doc nut nhan chong doi phím
  int reading = digitalRead(PIN_BUTTON);
  bool buttonPressed = false;

  if (reading != lastButtonReading) {
    lastDebounceTime = currentMillis;
  }

  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) {
      buttonPressed = true;
    }
    buttonState = reading;
  }
  lastButtonReading = reading;

  // 2. May trang thai
  switch (currentState) {
    case STATE_RUNNING:
      if (buttonPressed) {
        // Dung khan cap
        analogWrite(PIN_ENA, 0);
        digitalWrite(PIN_LED_GREEN, LOW);
        currentState = STATE_EMERGENCY;
      }
      break;

    case STATE_EMERGENCY:
      // LED do chop tat va buzzer canh bao dinh ky moi 400ms
      if (currentMillis - lastBlinkTime >= 400) {
        lastBlinkTime = currentMillis;
        blinkFlag = !blinkFlag;
        digitalWrite(PIN_LED_RED, blinkFlag);
        digitalWrite(PIN_BUZZER, blinkFlag);
      }

      if (buttonPressed) {
        // Chuyen sang giai doan chuan bi khoi dong lai trong 3s
        digitalWrite(PIN_LED_RED, LOW);
        digitalWrite(PIN_BUZZER, LOW);
        stateStartTime = currentMillis;
        currentState = STATE_RESTARTING;
      }
      break;

    case STATE_RESTARTING:
      // Trong 3s, neu nhan nut -> huy va quay lai trang thai dung khan cap
      if (buttonPressed) {
        digitalWrite(PIN_LED_YEL, LOW);
        currentState = STATE_EMERGENCY;
        break;
      }

      // Nhap nhay LED vang moi 200ms
      if (currentMillis - lastBlinkTime >= 200) {
        lastBlinkTime = currentMillis;
        blinkFlag = !blinkFlag;
        digitalWrite(PIN_LED_YEL, blinkFlag);
      }

      // Het 3 giay -> Tu dong khoi dong lai motor
      if (currentMillis - stateStartTime >= 3000) {
        digitalWrite(PIN_LED_YEL, LOW);
        digitalWrite(PIN_LED_GREEN, HIGH);
        analogWrite(PIN_ENA, 255);
        currentState = STATE_RUNNING;
      }
      break;
  }
}