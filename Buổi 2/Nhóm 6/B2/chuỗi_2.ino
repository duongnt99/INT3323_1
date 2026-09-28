const int led[3] = {25, 26, 27};
const int sequence[7] = {0, 1, 2,, 0, 1, 2};

void setup() {
  for (int i = 0; i <= 2; i++) {
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], LOW);
  }
}

void loop() {
    for (int i = 0; i <=6; i++) {
    digitalWrite(led[sequence[i]], HIGH);
    delay(1000);
    digitalWrite(led[sequence[i]], LOW);
    }
}