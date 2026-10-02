#define GREEN 16
#define RED 18
#define YELLOW 17
#define BUZZER 15

enum Sequence {
  Prepare,
  Warning,
  Launch,
  Reset
};

int countdown = 0;
unsigned long startTime = 0;
unsigned long previousBuzzer = 0;
unsigned long buzzerDelay = 0;
bool buzzerOn = false;
Sequence seq = Prepare;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RED, HIGH);
  startTime = millis();
}

void loop() {
  switch (seq) {
    case Prepare:
      prepare();
      break;

    case Warning:
      warning();
      break;
    case Launch:
      launch();
      break;
    case Reset:
      reset();
      break;
  }
}

void prepare() {
  digitalWrite(RED, HIGH);
  delay(2000);

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, HIGH);
  seq = Warning;
  startTime = millis();
  buzzerDelay = 1000;
  countdown = 5;
}

void warning() {
  unsigned long elapse = millis() - startTime;
  if (elapse > 1000) {
    startTime = millis();
    countdown--;
    if (countdown == 0) {
      seq = Launch;
      countdown = 5;
      return;
    }

    if (countdown <= 3) {
      buzzerDelay /= 2;
    }
  }

  unsigned long buzzerElapse = millis() - previousBuzzer;

  if (buzzerOn) {
    if (buzzerElapse > 100) {
      buzzerOn = false;
      noTone(BUZZER);
      digitalWrite(YELLOW, LOW);
    }
    return;
  }

  if (countdown > 3) {
    if (buzzerElapse > 1000) {
      buzzerOn = true;
      tone(BUZZER, 1000);
      digitalWrite(YELLOW, HIGH);
      previousBuzzer = millis();
    }
  } else {
    if (buzzerElapse > buzzerDelay) {
      buzzerOn = true;
      tone(BUZZER, 1000);
      digitalWrite(YELLOW, HIGH);
      previousBuzzer = millis();
    }
  }
}

void launch() {
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED, HIGH);
  tone(BUZZER, 1000);

  delay(500);

  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);

  delay(500);

  countdown--;

  if (countdown == 0) {
    seq = Reset;
  }
}

void reset() {
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
  noTone(BUZZER);

  delay(5000);
  seq = Prepare;
}