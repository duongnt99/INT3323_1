
const int BUTTON = 19;
const int LED1 = 21; //đỏ
const int LED2 = 22; //vàng
const int LED3 = 23; //xanh

int currentLed = LED1;

void deactivateAllLed() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  deactivateAllLed();
  digitalWrite(LED1, HIGH);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (currentLed == LED1) {
      currentLed = LED2;
    } else if (currentLed == LED2) {
      currentLed = LED3;
    } else {
      currentLed = LED1;
    }

    deactivateAllLed();
    digitalWrite(currentLed, HIGH);

    delay(50);

    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(50);
    }
  }
}
