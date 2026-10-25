#include <Arduino.h>
#include <DHT.h>
#define DHT_TYPE DHT11
const int GREEN_LED  = 25;
const int YELLOW_LED = 26;
const int RED_LED    = 27;
const int BUZZER     = 13;

const int SOIL       = 34;
const int MQ2_AO     = 32;
const int MQ2_DO     = 35;
const int DHT_PIN    = 14;

DHT dht(DHT_PIN, DHT_TYPE);

enum State {
  NORMAL,
  WATCH,
  HOT_AND_DRY,
  MQ2_ALERT,
  SENSOR_ERROR
};

State state = NORMAL;
int baselineMQ2 = 0;
unsigned long lastRead = 0;

void setLeds(bool r, bool y, bool g) {
  digitalWrite(RED_LED, r);
  digitalWrite(YELLOW_LED, y);
  digitalWrite(GREEN_LED, g);
}

void setup() {
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MQ2_DO, INPUT);

  digitalWrite(BUZZER, HIGH);
  setLeds(LOW, LOW, LOW);
  dht.begin();

  delay(2000);
  baselineMQ2 = analogRead(MQ2_AO);
}

void loop() {
  unsigned long now = millis();

  if (now - lastRead >= 2000) {
    lastRead = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int mq2AO = analogRead(MQ2_AO);
    int mq2DO = digitalRead(MQ2_DO);
    int rawSoil = analogRead(SOIL);

    float dryness = constrain((float)(rawSoil - 1500) / (3500 - 1500) * 100.0, 0.0, 100.0);
    int deltaMQ2 = max(0, mq2AO - baselineMQ2);
    bool dhtErr = isnan(h) || isnan(t);

    if (deltaMQ2 >= 300 || mq2DO == LOW) {
      state = MQ2_ALERT;
    } else if (!dhtErr && t >= 30.0 && dryness >= 70.0) {
      state = HOT_AND_DRY;
    } else if (dhtErr) {
      state = SENSOR_ERROR;
    } else if (dryness >= 55.0 || t >= 30.0 || h < 40.0) {
      state = WATCH;
    } else {
      state = NORMAL;
    }

    if (dhtErr) {
      Serial.println("Khong the doc du lieu tu cam bien DHT11!");
    } else {
      Serial.printf("Nhiet do: %.1f °C | Do am KK: %.1f %%\n", t, h);
    }
    Serial.printf("Do kho dat: %.1f %% | MQ2 AO: %d (+%d) | MQ2 DO: %d\n", dryness, mq2AO, deltaMQ2, mq2DO);

    Serial.print("Trang thai : ");
    switch (state) {
      case MQ2_ALERT:    Serial.println("MQ2 ALERT"); break;
      case HOT_AND_DRY:  Serial.println("HOT & DRY"); break;
      case WATCH:        Serial.println("WATCH"); break;
      case NORMAL:       Serial.println("NORMAL"); break;
      case SENSOR_ERROR: Serial.println("SENSOR ERROR"); break;
    }
  }

  bool blinkFast = (now / 100) % 2;
  bool blinkSlow = (now / 500) % 2;
  bool beepShort = (now % 1000) < 200;

  switch (state) {
    case MQ2_ALERT:
      setLeds(blinkFast, LOW, LOW);
      digitalWrite(BUZZER, blinkFast ? LOW : HIGH);
      break;

    case HOT_AND_DRY:
      setLeds(HIGH, LOW, LOW);
      digitalWrite(BUZZER, beepShort ? LOW : HIGH);
      break;

    case WATCH:
    case SENSOR_ERROR:
      setLeds(LOW, blinkSlow, LOW);
      digitalWrite(BUZZER, HIGH);
      break;

    case NORMAL:
      setLeds(LOW, LOW, HIGH);
      digitalWrite(BUZZER, HIGH);
      break;
  }
}