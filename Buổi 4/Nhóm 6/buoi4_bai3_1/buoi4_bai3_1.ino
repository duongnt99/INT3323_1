
const int BUTTON = 14;

const int LED_GREEN  = 27;
const int LED_YELLOW = 33;
const int LED_RED    = 32;

const int BUZZER = 22;

const int ENA = 25;
const int IN1 = 26;
const int IN2 = 18;


const int PWM_LEVEL1 = 120;
const int PWM_LEVEL2 = 180;
const int PWM_LEVEL3 = 255;


int fanLevel = 0;


bool buttonClicked() {
  static int lastRaw = HIGH;
  static int stable = HIGH;
  static unsigned long changedAt = 0;

  int raw = digitalRead(BUTTON);

  if (raw != lastRaw) {
    changedAt = millis();
  }

  lastRaw = raw;

  if (millis() - changedAt >= 40 && raw != stable) {
    stable = raw;
    return stable == LOW;
  }

  return false;
}


void ledsOff() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
}


void beep(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(duration);

    digitalWrite(BUZZER, LOW);

    if (i < times - 1) {
      delay(200);
    }
  }
}


void updateFan() {
  ledsOff();

  switch (fanLevel) {
    case 0:
      analogWrite(ENA, 0);
      Serial.println("FAN OFF");
      break;

    case 1:
      analogWrite(ENA, PWM_LEVEL1);
      digitalWrite(LED_GREEN, HIGH);
      Serial.println("FAN LEVEL 1");
      break;

    case 2:
      analogWrite(ENA, PWM_LEVEL2);
      digitalWrite(LED_YELLOW, HIGH);
      Serial.println("FAN LEVEL 2");
      break;

    case 3:
      analogWrite(ENA, PWM_LEVEL3);
      digitalWrite(LED_RED, HIGH);
      Serial.println("FAN LEVEL 3");
      break;
  }
}

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);


  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(BUZZER, LOW);

  Serial.begin(115200);

  updateFan();
}

void loop() {
  if (buttonClicked()) {
    fanLevel++;

    if (fanLevel > 3) {
      fanLevel = 0;
    }

    updateFan();

    if (fanLevel == 0) {
      beep(1, 1000);
    } else {
      beep(fanLevel, 100);
    }
  }
}
