#define GREEN_PIN 16
#define RED_PIN 18
#define YELLOW_PIN 17
#define BUZZER_PIN 15

enum Sequence {
  GREEN, YELLOW, RED
};

unsigned long startTime = 0;
unsigned long previousBuzzer = 0;
bool buzzerOn = false;
Sequence seq = GREEN;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(GREEN_PIN, HIGH);
  seq = GREEN;
  startTime = millis();
}

void loop() {
  switch(seq) {
    case GREEN:
      handleGreen();
      break;

    case YELLOW:
      handleYellow();
      break;

    case RED:
      handleRed();
      break;
  }
}

void handleYellow() 
{
  if(millis() - startTime > 2000) {
    seq = RED;
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(RED_PIN, HIGH);
    startTime = millis();
  }
}

void handleGreen()
{
  unsigned long time = millis();

  if(time - startTime < 3000) {
    return;
  }

  if(time - startTime > 5000) {
    seq = YELLOW;
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
    startTime = millis();
  }

  if(!buzzerOn && time - previousBuzzer > 500) {
    previousBuzzer = time;
    buzzerOn = true;
    tone(BUZZER_PIN, 1000);
    return;
  }

  if(buzzerOn && time - previousBuzzer > 250) {
    buzzerOn = false;
    noTone(BUZZER_PIN);
  }
}


void handleRed()
{
  unsigned long time = millis();

  if(time - startTime < 2000) {
    return;
  }

  if(time - startTime > 5000) {
    seq = GREEN;
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    startTime = millis();
  }

  if(!buzzerOn && time - previousBuzzer > 1000) {
    previousBuzzer = time;
    buzzerOn = true;
    tone(BUZZER_PIN, 1000);
    return;
  }

  if(buzzerOn && time - previousBuzzer > 500) {
    buzzerOn = false;
    noTone(BUZZER_PIN);
  }
}
