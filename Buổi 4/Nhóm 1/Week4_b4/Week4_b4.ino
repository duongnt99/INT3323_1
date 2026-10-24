
const int BUTTON = 14;

const int LED_RED    = 27;
const int LED_YELLOW = 33;
const int LED_GREEN  = 32;

const int BUZZER = 22;

// L298N
const int ENA = 25;
const int IN1 = 26;
const int IN2 = 18;

const int MOTOR_PWM = 200;

// Cac trang thai he thong
enum State {
  RUNNING,
  EMERGENCY_STOP,
  RESTART_WAIT
};

State state = RUNNING;

// Bien thoi gian
unsigned long stateStart = 0;
unsigned long ledTime = 0;
unsigned long buzzerTime = 0;

bool ledState = false;
bool buzzerState = false;

// Phat hien mot lan nhan, chong doi nut
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

// Dieu khien motor
void motorRun() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, MOTOR_PWM);
}

void motorStop() {
  analogWrite(ENA, 0);
}

// Tat tat ca LED
void allLedsOff() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
}

// Trang thai dung khan cap
void emergencyStop() {
  motorStop();
  allLedsOff();

  ledState = false;
  buzzerState = false;

  digitalWrite(BUZZER, LOW);

  ledTime = millis();
  buzzerTime = millis();

  state = EMERGENCY_STOP;

  Serial.println("EMERGENCY STOP");
}

// Trang thai chuan bi khoi dong lai
void prepareRestart() {
  motorStop();
  allLedsOff();

  ledState = false;
  digitalWrite(BUZZER, LOW);

  stateStart = millis();
  ledTime = millis();

  state = RESTART_WAIT;

  Serial.println("RESTART PREPARATION");
}

// Cap nhat he thong khong dung delay()
void updateSystem() {
  unsigned long now = millis();

  switch (state) {
    case RUNNING:
      motorRun();

      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(BUZZER, LOW);
      break;

    case EMERGENCY_STOP:
      motorStop();

      // LED do nhap nhay moi 500 ms
      if (now - ledTime >= 500) {
        ledTime = now;
        ledState = !ledState;
        digitalWrite(LED_RED, ledState);
      }

      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);

      // Buzzer canh bao dinh ky
      if (now - buzzerTime >= 1000) {
        buzzerTime = now;
        buzzerState = !buzzerState;
        digitalWrite(BUZZER, buzzerState);
      }
      break;

    case RESTART_WAIT:
      motorStop();

      // LED vang nhap nhay moi 250 ms
      if (now - ledTime >= 250) {
        ledTime = now;
        ledState = !ledState;
        digitalWrite(LED_YELLOW, ledState);
      }

      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(BUZZER, LOW);

      // Cho du 3 giay roi khoi dong lai
      if (now - stateStart >= 3000) {
        state = RUNNING;
        allLedsOff();

        digitalWrite(LED_GREEN, HIGH);

        motorRun();

        Serial.println("MOTOR RESTARTED");
      }
      break;
  }
}

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(115200);

  // Khoi dong he thong
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  motorRun();

  allLedsOff();
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(BUZZER, LOW);

  Serial.println("SYSTEM RUNNING");
}

void loop() {
  // Xu ly nut nhan
  if (buttonClicked()) {
    if (state == RUNNING) {
      emergencyStop();
    }
    else if (state == EMERGENCY_STOP) {
      prepareRestart();
    }
    else if (state == RESTART_WAIT) {
      emergencyStop();
    }
  }

  // Cap nhat LED, buzzer va motor
  updateSystem();
}
