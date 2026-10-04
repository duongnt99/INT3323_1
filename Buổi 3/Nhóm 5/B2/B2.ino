#define GREEN_PIN 16
#define RED_PIN 18
#define YELLOW_PIN 17
#define BUZZER_PIN 15

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  delay(3000);

  digitalWrite(BUZZER_PIN, HIGH);
  delay(250);
  digitalWrite(BUZZER_PIN, LOW);

  delay(250);

  digitalWrite(BUZZER_PIN, HIGH);
  delay(250);
  digitalWrite(BUZZER_PIN, LOW);

  delay(2000);

  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, HIGH);

  delay(2000);

  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(RED_PIN, HIGH);

  delay(2000);

  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);

  delay(500);

  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);

  delay(500);

  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);

  delay(1000);

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
}