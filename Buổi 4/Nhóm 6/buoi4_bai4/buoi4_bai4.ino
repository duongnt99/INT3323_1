
const int BUTTON = 14;

const int LED_RED    = 27;
const int LED_YELLOW = 33;
const int LED_GREEN  = 32;

const int BUZZER = 22;


const int ENA = 25;
const int IN1 = 26;
const int IN2 = 18;

const int MOTOR_PWM = 200;


enum State {
  RUNNING,
  EMERGENCY_STOP,
  RESTART_WAIT
};

State state = RUNNING;


unsigned long stateStart = 0;
unsigned long ledTime = 0;
unsigned long buzzerTime = 0;

bool ledState = false;
bool buzzerState = false;


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


void motorRun() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, MOTOR_PWM);
}

void motorStop() {
  analogWrite(ENA, 0);
}


void allLedsOff() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
}


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


      if (now - ledTime >= 500) {
        ledTime = now;
        ledState = !ledState;
        digitalWrite(LED_RED, ledState);
      }

      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);


      if (now - buzzerTime >= 1000) {
        buzzerTime = now;
        buzzerState = !buzzerState;
        digitalWrite(BUZZER, buzzerState);
      }
      break;

    case RESTART_WAIT:
      motorStop();


      if (now - ledTime >= 250) {
        ledTime = now;
        ledState = !ledState;
        digitalWrite(LED_YELLOW, ledState);
      }

      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(BUZZER, LOW);


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


  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  motorRun();

  allLedsOff();
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(BUZZER, LOW);

  Serial.println("SYSTEM RUNNING");
}

void loop() {

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


  updateSystem();
}
