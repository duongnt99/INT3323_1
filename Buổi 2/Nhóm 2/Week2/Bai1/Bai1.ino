#define LED_RED_PIN 23
#define LED_YELLOW_PIN 22

void setup() {
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(LED_YELLOW_PIN, LOW);
  delay(1000);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  digitalWrite(LED_RED_PIN, LOW);
  delay(1000);
}
