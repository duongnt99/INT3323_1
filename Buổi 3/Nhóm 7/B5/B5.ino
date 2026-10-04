const int LED1 = 25;
const int BUTTON = 27;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  digitalWrite(LED1, LOW);
}

void loop() {

  if (digitalRead(BUTTON) == LOW) {
    buttonState = !buttonState;
    digitalWrite(LED1, buttonState);
    delay(50);
    while (digitalRead(BUTTON) == LOW)
      delay(50);
  }
}