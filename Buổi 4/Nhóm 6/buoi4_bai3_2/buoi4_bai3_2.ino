
const int BUTTON = 14;

const int LED_RED    = 27;
const int LED_YELLOW = 33;
const int LED_GREEN  = 32;

const int BUZZER = 22;


const int ENA = 25;
const int IN1 = 26;
const int IN2 = 18;


const int MOTOR_TIME = 5000;


const int MOTOR_PWM = 200;

bool running = false;


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

void motorStop() {
  analogWrite(ENA, 0);
}

void motorRun() {
  analogWrite(ENA, MOTOR_PWM);
}

void allLedsOff() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void readyState() {
  motorStop();
  allLedsOff();

  digitalWrite(LED_RED, HIGH);
  digitalWrite(BUZZER, LOW);

  Serial.println("SYSTEM READY");
}

void countdown() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_GREEN, LOW);

  Serial.println("COUNTDOWN");

  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(200);

    digitalWrite(BUZZER, LOW);
    delay(800);
  }
}

void conveyorRun() {
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, HIGH);

  motorRun();

  Serial.println("CONVEYOR RUNNING");
  delay(MOTOR_TIME);

  motorStop();

  Serial.println("CONVEYOR STOPPED");
}

void completedSignal() {
  digitalWrite(LED_GREEN, LOW);

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_GREEN, HIGH);
    delay(300);

    digitalWrite(LED_GREEN, LOW);
    delay(300);
  }

  Serial.println("PACKAGING CYCLE COMPLETED");
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

  readyState();
}

void loop() {
  if (buttonClicked() && !running) {
    running = true;

    countdown();
    conveyorRun();
    completedSignal();

    running = false;
    readyState();
  }
}
