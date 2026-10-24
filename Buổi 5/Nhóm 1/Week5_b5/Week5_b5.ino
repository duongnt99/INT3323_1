
#include <DHT.h>

// ===== KHAI BAO CHAN =====
#define DHT_PIN 16
#define DHT_TYPE DHT11

const int SOIL_AO = 35;
const int MQ_AO = 34;
const int MQ_DO = 18;

const int LED_GREEN  = 25;
const int LED_YELLOW = 26;
const int LED_RED    = 27;
const int BUZZER     = 22;

DHT dht(DHT_PIN, DHT_TYPE);

// ===== HIEU CHUAN =====
int wetValue = -1;
int dryValue = -1;
int mqBaseline = -1;

bool wetReady = false;
bool dryReady = false;
bool mqReady = false;

// ===== DU LIEU CAM BIEN =====
float temperature = NAN;
float humidity = NAN;
float dryness = NAN;

int soilADC = 0;
int mqADC = 0;
int mqDigital = 0;
int mqIncrease = 0;

bool dhtValid = false;
bool soilReady = false;

// ===== THOI GIAN =====
const unsigned long SAMPLE_INTERVAL = 2000;

unsigned long lastSampleTime = 0;
unsigned long lastBlinkTime = 0;
unsigned long lastBeepTime = 0;

bool ledBlinkState = false;
bool buzzerState = false;

// ===== TRANG THAI =====
enum State {
  WAIT_CALIBRATION,
  SENSOR_ERROR,
  NORMAL,
  WATCH,
  HOT_DRY,
  MQ_ALERT
};

State currentState = WAIT_CALIBRATION;

// Doc trung binh ADC, khong dung delay()
int readAverageADC(int pin, int samples = 20) {
  long total = 0;

  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
  }

  return total / samples;
}

void allOutputsOff() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER, LOW);

  ledBlinkState = false;
  buzzerState = false;
}

void printStatus(const char* status) {
  Serial.print("STATUS: ");
  Serial.println(status);
}

void processSerialCommand() {
  while (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'w' || command == 'W') {
      wetValue = readAverageADC(SOIL_AO);
      wetReady = true;

      Serial.print("Wet reference: ");
      Serial.println(wetValue);
    }
    else if (command == 'd' || command == 'D') {
      dryValue = readAverageADC(SOIL_AO);
      dryReady = true;

      Serial.print("Dry reference: ");
      Serial.println(dryValue);
    }
    else if (command == 'b' || command == 'B') {
      mqBaseline = readAverageADC(MQ_AO, 50);
      mqReady = true;

      Serial.print("MQ2 baseline: ");
      Serial.println(mqBaseline);
    }

    if (wetReady && dryReady && wetValue == dryValue) {
      Serial.println("Soil calibration error: values are equal.");
      wetReady = false;
      dryReady = false;
    }
  }
}

void readSensors() {
  // DHT11
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  dhtValid = !isnan(t) && !isnan(h);

  if (dhtValid) {
    temperature = t;
    humidity = h;
  }

  // Cam bien dat
  soilADC = analogRead(SOIL_AO);

  soilReady = wetReady && dryReady;

  if (soilReady) {
    dryness = 100.0 *
        (soilADC - wetValue) /
        (float)(dryValue - wetValue);

    dryness = constrain(dryness, 0.0, 100.0);
  }

  // MQ2
  mqADC = analogRead(MQ_AO);
  mqDigital = digitalRead(MQ_DO);

  if (mqReady) {
    mqIncrease = mqADC - mqBaseline;

    if (mqIncrease < 0) {
      mqIncrease = 0;
    }
  }
}

void printSensorData() {
  Serial.println("\n--- GREENHOUSE GUARDIAN ---");

  if (dhtValid) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Air humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println("SENSOR ERROR: DHT11 read failed!");
  }

  Serial.print("Soil AO: ");
  Serial.println(soilADC);

  if (soilReady) {
    Serial.print("Soil dryness: ");
    Serial.print(dryness, 1);
    Serial.println(" %");
  } else {
    Serial.println("Soil calibration pending.");
  }

  Serial.print("MQ2 AO: ");
  Serial.println(mqADC);

  Serial.print("MQ2 DO: ");
  Serial.println(mqDigital);

  if (mqReady) {
    Serial.print("MQ2 baseline: ");
    Serial.println(mqBaseline);

    Serial.print("MQ2 increase: ");
    Serial.println(mqIncrease);
  } else {
    Serial.println("MQ2 baseline pending.");
  }
}

void determineState() {
  bool ready = soilReady && mqReady;

  if (!ready) {
    currentState = WAIT_CALIBRATION;
    return;
  }

  // Uu tien 1: MQ2 ALERT
  if (mqIncrease >= 400) {
    currentState = MQ_ALERT;
    return;
  }

  // Uu tien 2: HOT & DRY
  if (dhtValid &&
      dryness >= 70 &&
      temperature >= 30) {
    currentState = HOT_DRY;
    return;
  }

  // Neu DHT loi, khong ket luan NORMAL
  if (!dhtValid) {
    currentState = SENSOR_ERROR;
    return;
  }

  // Uu tien 3: WATCH
  if (mqIncrease >= 150 ||
      dryness >= 55 ||
      temperature >= 30 ||
      humidity < 40 ||
      humidity > 75) {
    currentState = WATCH;
    return;
  }

  // Uu tien 4: NORMAL
  currentState = NORMAL;
}

void printReasons() {
  switch (currentState) {
    case MQ_ALERT:
      printStatus("MQ2 ALERT");
      Serial.println("Reason: MQ2 increase >= 400 ADC");
      break;

    case HOT_DRY:
      printStatus("HOT & DRY");
      Serial.println("Reason: Temperature >= 30 C AND soil dryness >= 70%");
      break;

    case WATCH:
      printStatus("WATCH");

      if (mqIncrease >= 150) {
        Serial.println("- MQ2 increase >= 150 ADC");
      }

      if (dryness >= 55) {
        Serial.println("- Soil dryness >= 55%");
      }

      if (temperature >= 30) {
        Serial.println("- Temperature >= 30 C");
      }

      if (humidity < 40) {
        Serial.println("- Air humidity < 40%");
      }

      if (humidity > 