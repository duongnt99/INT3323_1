
const int BUTTON = 14;

const int LED_RED    = 27;
const int LED_YELLOW = 33;
const int LED_GREEN  = 32;

const int BUZZER = 22;


const int ENA = 25;
const int IN1 = 26;
const int IN2 = 18;

const int MOTOR_PWM = 200;


enum GameState {
  WAITING,
  RUNNING,
  ALERT,
  SUCCESS,
  FAILURE
};

GameState state = WAITING;

unsigned long stateStart = 0;
unsigned long ledTime = 0;
unsigned long buzzerTime = 0;

unsigned long randomDelay = 0;

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

void startGame() {
  state = RUNNING;
  stateStart = millis();


  randomDelay = random(3000, 6001);

  motorRun();
  allLedsOff();

  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(BUZZER, LOW);

  Serial.println("GAME STARTED");
}

void showSuccess() {
  state = SUCCESS;
  stateStart = millis();
  ledTime = millis();

  motorStop();
  allLedsOff();

  ledState = false;
  digitalWrite(BUZZER, LOW);

  Serial.println("SUCCESS!");
}

void showFailure() {
  state = FAILURE;
  stateStart = millis();
  ledTime = millis();
  buzzerTime = millis();

  motorStop();
  allLedsOff();

  ledState = false;
  buzzerState = false;

  digitalWrite(BUZZER, LOW);

  Serial.println("FAILURE!");
}

void resetGame() {
  state = WAITING;

  motorStop();
  allLedsOff();

  digitalWrite(LED_RED, HIGH);
  digitalWrite(BUZZER, LOW);

  Serial.println("READY - PRESS START");
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

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(BUZZER, LOW);

  Serial.begin(115200);


  randomSeed(micros());

  resetGame();
}

void loop() {
  unsigned long now = millis();
  bool pressed = buttonClicked();

  switch (state) {
    case WAITING:
      if (pressed) {
        startGame();
      }
      break;

    case RUNNING:

      if (pressed) {
        showFailure();
      }
      else if (now - stateStart >= randomDelay) {
        state = ALERT;
        stateStart = now;
        ledTime = now;
        buzzerTime = now;

        ledState = false;
        buzzerState = false;

        Serial.println("ALERT! PRESS NOW!");
      }
      break;

    case ALERT:

      if (now - ledTime >= 200) {
        ledTime = now;
        ledState = !ledState;
        digitalWrite(LED_YELLOW, ledState);
      }


      if (now - buzzerTime >= 250) {
        buzzerTime = now;
        buzzerState = !buzzerState;
        digitalWrite(BUZZER, buzzerState);
      }


      if (pressed) {
        if (now - stateStart <= 2000) {
          showSuccess();
        } else {
          showFailure();
        }
      }
      else if (now - stateStart >= 2000) {
        showFailure();
      }
      break;

    case SUCCESS:

      if (now - ledTime >= 200) {
        ledTime = now;
        ledState = !ledState;
        digitalWrite(LED_GREEN, ledState);
      }

      if (now - stateStart >= 2000) {
        resetGame();
      }
      break;

    case FAILURE:

      if (now - ledTime >= 200) {
        ledTime = now;
        ledState = !ledState;
        digitalWrite(LED_RED, ledState);
      }


      if (now - buzzerTime >= 250) {
        buzzerTime = now;
        buzzerState = !buzzerState;
        digitalWrite(BUZZER, buzzerState);
      }


      if (now - stateStart >= 2000) {
        resetGame();
      }
      break;
  }
}
