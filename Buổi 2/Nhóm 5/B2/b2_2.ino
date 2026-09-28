const int ledPin[] = {21, 22, 23};
const int numLed = sizeof(ledPin) / sizeof(ledPin[0]);
const int delayTime = 1000; // mỗi đèn sáng 1 giây

void setup() {
  for (int i = 0; i < numLed; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
}

void loop() {
  // đèn sáng theo thứ tự chuỗi sáng 2: 21>22>23
  for (int i = 0; i < numLed; i++) {
    for (int j = 0; j < numLed; j++) {
      digitalWrite(ledPin[j], LOW);
    }
    digitalWrite(ledPin[i], HIGH);
    delay(delayTime);
  }
}