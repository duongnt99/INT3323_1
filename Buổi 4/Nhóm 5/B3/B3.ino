// B3: Quat ba cap toc do co bao am Buzzer
const int PIN_BUTTON    = 18;
const int PIN_LED_GREEN = 21;
const int PIN_LED_YEL   = 22;
const int PIN_LED_RED   = 19;
const int PIN_BUZZER    = 4;
const int PIN_ENA       = 25;
const int PIN_IN1       = 26;
const int PIN_IN2       = 27;

// Tuy chinh gia tri PWM cho tung muc toc do
const int PWM_LV0 = 0;
const int PWM_LV1 = 140;
const int PWM_LV2 = 200;
const int PWM_LV3 = 255;

int fanLevel = 0; // 0: Tat, 1: Muc 1, 2: Muc 2, 3: Muc 3
int lastButtonReading = HIGH;
int buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void playBeep(int count, int durationMs) {
  for (int i = 0; i < count; i++) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(durationMs);
    digitalWrite(PIN_BUZZER, LOW);
    if (count > 1 && i < count - 1) delay(100);
  }
}

void applySpeed(int level) {
  // Tat toan bo LED
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_YEL, LOW);
  digitalWrite(PIN_LED_RED, LOW);

  switch (level) {
    case 1:
      digitalWrite(PIN_LED_GREEN, HIGH);
      analogWrite(PIN_ENA, PWM_LV1);
      playBeep(1, 100); // 1 tieng ngan
      break;
    case 2:
      digitalWrite(PIN_LED_YEL, HIGH);
      analogWrite(PIN_ENA, PWM_LV2);
      playBeep(2, 100); // 2 tieng ngan
      break;
    case 3:
      digitalWrite(PIN_LED_RED, HIGH);
      analogWrite(PIN_ENA, PWM_LV3);
      playBeep(3, 100); // 3 tieng ngan
      break;
    case 0:
    default:
      analogWrite(PIN_ENA, PWM_LV0);
      playBeep(1, 600); // 1 tieng dai khi tat
      break;
  }
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_YEL, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);

  // Khoi dong: dung motor, tat LED
  applySpeed(0);
}

void loop() {
  int reading = digitalRead(PIN_BUTTON);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) {
      fanLevel = (fanLevel + 1) % 4; // Chuyen cap tu 0 -> 1 -> 2 -> 3 -> 0
      applySpeed(fanLevel);
    }
    buttonState = reading;
  }

  lastButtonReading = reading;
}