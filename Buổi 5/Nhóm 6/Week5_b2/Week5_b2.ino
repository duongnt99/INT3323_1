
const int SOIL_AO = 35;
const int SOIL_DO = 19;

const int LED_GREEN  = 25;
const int LED_YELLOW = 26;
const int LED_RED    = 27;
const int BUZZER     = 22;

int wetValue = -1;
int dryValue = -1;

bool calibratedWet = false;
bool calibratedDry = false;

int readAverageADC() {
  long total = 0;

  for (int i = 0; i < 20; i++) {
    total += analogRead(SOIL_AO);
    delay(20);
  }

  return total / 20;
}

void calibrateSensor(char command) {
  int value = readAverageADC();

  if (command == 'w' || command == 'W') {
    wetValue = value;
    calibratedWet = true;

    Serial.print("Wet reference = ");
    Serial.println(wetValue);
  }
  else if (command == 'd' || command == 'D') {
    dryValue = value;
    calibratedDry = true;

    Serial.print("Dry reference = ");
    Serial.println(dryValue);
  }

  if (calibratedWet && calibratedDry) {
    if (wetValue == dryValue) {
      Serial.println("CALIBRATION ERROR: Values are equal.");
      calibratedWet = false;
      calibratedDry = false;
    } else {
      Serial.println("Calibration completed!");
    }
  }
}

void allLEDsOff() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(SOIL_DO, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  analogReadResolution(12);

  allLEDsOff();
  digitalWrite(BUZZER, LOW);

  Serial.println("Smart Plant Pot");
  Serial.println("1. Put sensor in very wet soil, send: w");
  Serial.println("2. Put sensor in dry soil, send: d");
  Serial.println("Use Serial Monitor: 9600 baud");
}

void loop() {

  if (!calibratedWet || !calibratedDry) {
    if (Serial.available() > 0) {
      char command = Serial.read();

      if (command == 'w' || command == 'W' ||
          command == 'd' || command == 'D') {
        calibrateSensor(command);
      }
    }
    return;
  }

  int adcValue = analogRead(SOIL_AO);
  int digitalValue = digitalRead(SOIL_DO);


  float dryness = 100.0 *
      (adcValue - wetValue) /
      (float)(dryValue - wetValue);

  dryness = constrain(dryness, 0.0, 100.0);

  allLEDsOff();
  digitalWrite(BUZZER, LOW);

  Serial.print("ADC: ");
  Serial.print(adcValue);

  Serial.print(" | DO: ");
  Serial.print(digitalValue);

  Serial.print(" | Dryness: ");
  Serial.print(dryness, 1);
  Serial.print("% | Status: ");

  if (dryness < 35) {
    digitalWrite(LED_GREEN, HIGH);
    Serial.println("WET");
  }
  else if (dryness < 70) {
    digitalWrite(LED_YELLOW, HIGH);
    Serial.println("MODERATE");
  }
  else {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER, HIGH);
    Serial.println("DRY - WATER NEEDED!");
  }

  delay(1000);
}
