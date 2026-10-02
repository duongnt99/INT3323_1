#define LED_RED_PIN 22
#define LED_YELLOW_PIN 23
#define LED_GREEN_PIN 17

// RED -> YELLOW -> GREEN -> YELLOW -> RED -> YELLOW -> ...
const int SEQUENCE_COUNT = 4;
const int SEQUENCE[SEQUENCE_COUNT] = {
  LED_RED_PIN,
  LED_YELLOW_PIN,
  LED_GREEN_PIN,
  LED_YELLOW_PIN
};

// Alternate sequence: RED -> YELLOW -> GREEN -> RED -> YELLOW -> ...
// const int SEQUENCE_COUNT = 3;
// const int SEQUENCE[SEQUENCE_COUNT] = {
//   LED_RED_PIN,
//   LED_YELLOW_PIN,
//   LED_GREEN_PIN
// };

int previous = SEQUENCE[0];

void setup() {
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
}

void loop() {
  for(int i = 0; i < SEQUENCE_COUNT; i++){
    digitalWrite(previous, LOW);
    digitalWrite(SEQUENCE[i], HIGH);
    delay(1000);
    previous = SEQUENCE[i];
  }
}
