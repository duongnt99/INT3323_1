int LED1 = 25;
int BUTTON = 26;

bool buttonState = false;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
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