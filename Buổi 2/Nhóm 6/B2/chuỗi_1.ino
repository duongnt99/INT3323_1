const int leds[3] = {26, 27, 25}; 
const int seq1[7] = {0, 1, 2, 1, 0, 1, 2};

void setup() {
  for (int i = 0; i <= 2; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  for (int i = 0; i <= 6; i++) {
    digitalWrite(leds[seq1[i]], HIGH);
    delay(1000);
    digitalWrite(leds[seq1[i]], LOW);
  }
}