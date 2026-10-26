// B2: Bat/Tat quat luan phien bang 1 nut nhan
const int PIN_BUTTON    = 18;
const int PIN_LED_RED   = 19;
const int PIN_LED_GREEN = 21;
const int PIN_ENA       = 25;
const int PIN_IN1       = 26;
const int PIN_IN2       = 27;

bool isFanOn = false;
int lastButtonReading = HIGH;
int buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setFanState(bool state) {
  if (state) {
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);
    analogWrite(PIN_ENA, 255);
    Serial.println("Trang thai quat: ON");
  } else {
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
    analogWrite(PIN_ENA, 0);
    Serial.println("Trang thai quat: OFF");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  // Thiet lap chieu quay
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);

  // Ban dau: quat dung, LED do sang
  setFanState(false);
}

void loop() {
  int reading = digitalRead(PIN_BUTTON);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Phat hien khi nut vua duoc nhan xuong (canh xuong)
    if (reading == LOW && buttonState == HIGH) {
      isFanOn = !isFanOn;
      setFanState(isFanOn);
    }
    buttonState = reading;
  }

  lastButtonReading = reading;
}