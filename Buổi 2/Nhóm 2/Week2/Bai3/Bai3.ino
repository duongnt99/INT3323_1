#define LED_RED_PIN 23
#define BUZZER_PIN 22

void setup() {
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);
}
