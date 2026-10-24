
const int MQ_AO = 34;
const int MQ_DO = 18;

const int LED_GREEN  = 25;
const int LED_YELLOW = 26;
const int LED_RED    = 27;
const int BUZZER     = 22;

int baseline = 0;
bool baselineReady = false;

int readAverageAO() {
  long total = 0;

  for (int i = 0; i < 50; i++) {
    total += analogRead(MQ_AO);
    delay(20);
  }

  return total / 50;
}

void allLEDsOff() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(MQ_DO, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  analogReadResolution(12);

  allLEDsOff();
  digitalWrite(BUZZER, LOW);

  Serial.println("Gas Detective Starting...");
  Serial.println("Warm up MQ-2 according to manufacturer instructions.");
  Serial.println("When stable, send 'b' to record baseline.");
}

void loop() {
  // Cho phep ghi lai baseline bang lenh b
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'b' || command == 'B') {
      Serial.println("Recording baseline...");
      baseline = readAverageAO();
      baselineReady = true;

      Serial.print("Baseline AO = ");
      Serial.println(baseline);
    }
  }

  int analogValue = analogRead(MQ_AO);
  int digitalValue = digitalRead(MQ_DO);

  Serial.print("AO: ");
  Serial.print(analogValue);

  Serial.print(" | DO: ");
  Serial.print(digitalValue);

  if (!baselineReady) {
    allLEDsOff();
    digitalWrite(BUZZER, LOW);
    Serial.println(" | Waiting for baseline...");
    delay(1000);
    return;
  }

  // Chi xet muc tang so voi baseline
  int increase = analogValue - baseline;
  if (increase < 0) {
    increase = 0;
  }

  allLEDsOff();
  digitalWrite(BUZZER, LOW);

  Serial.print(" | Baseline: ");
  Serial.print(baseline);

  Serial.print(" | Increase: ");
  Serial.print(increase);

  Serial.print(" | Status: ");

  if (increase < 150) {
    digitalWrite(LED_GREEN, HIGH);
    Serial.println("NORMAL");
  }
  else if (increase < 400) {
    digitalWrite(LED_YELLOW, HIGH);
    Serial.println("WARNING");
  }
  else {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER, HIGH);
    Serial.println("ALERT!");
  }

  delay(1000);
}
