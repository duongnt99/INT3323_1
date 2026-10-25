#include <DHT.h>
#define DHT_PIN 16
#define DHT_TYPE DHT11

const int SOIL_AO = 35;
const int SOIL_DO = 19;
const int MQ_AO = 34;
const int MQ_DO = 18;

const int LED_GREEN = 23;
const int LED_YELLOW = 22;
const int LED_RED = 25;
const int BUZZER_PIN = 32;

const int WET_VALUE = 1300;
const int DRY_VALUE = 3200;

const int MQ2_ALERT_THRESHOLD = 400;

DHT dht(DHT_PIN, DHT_TYPE);

int mq2_baseline = 0;

enum SystemState {
  STATE_NORMAL,
  STATE_WATCH,
  STATE_HOT_DRY,
  STATE_MQ2_ALERT
};

SystemState currentState = STATE_NORMAL;

unsigned long lastSensorReadTime = 0;
const unsigned long SENSOR_INTERVAL = 2000;

void turnOffAll(){
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, HIGH);
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(SOIL_DO, INPUT);
  pinMode(MQ_DO, INPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  analogReadResolution(12);

  Serial.println("Greenhouse System Starting...");
  Serial.println("Calibrating MQ2 Baseline...");
  delay(3000);

  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(MQ_AO);
    delay(100);
  }
  mq2_baseline = sum / 10;
  Serial.print("MQ2 Baseline initialized at: ");
  Serial.println(mq2_baseline);
  Serial.println("------------------------------------");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastSensorReadTime >= SENSOR_INTERVAL) {
    lastSensorReadTime = currentMillis;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    bool dhtError = isnan(humidity) || isnan(temperature);

    int rawSoil = analogRead(SOIL_AO);
    int soilDryness = map(rawSoil, WET_VALUE, DRY_VALUE, 0, 100);
    soilDryness = constrain(soilDryness, 0, 100);

    int mq2Analog = analogRead(MQ_AO);
    int mq2Digital = digitalRead(MQ_DO);
    int mq2Delta = mq2Analog - mq2_baseline;
    if (mq2Delta < 0) mq2Delta = 0;

    if (dhtError) Serial.print("ERR"); else Serial.print(temperature);
    Serial.print("Temp: ");
    Serial.print(" C | Hum: ");
    if (dhtError) Serial.print("ERR"); else Serial.print(humidity);
    Serial.print(" % | Soil Dryness: "); Serial.print(soilDryness);
    Serial.print("% | MQ2 AO: "); Serial.print(mq2Analog);
    Serial.print(" | MQ2 DO: "); Serial.println(mq2Digital);

    // 5. Kiểm tra lỗi DHT11
    if (dhtError) {
      Serial.println("SENSOR ERROR: Failed to read from DHT11!");
    }

    // 3 & 4. Xử lý trạng thái theo thứ tự ưu tiên: MQ2 ALERT -> HOT & DRY -> WATCH -> NORMAL
    if (mq2Delta >= MQ2_ALERT_THRESHOLD) {
      // MQ2 ALERT
      currentState = STATE_MQ2_ALERT;
      Serial.println("Status: MQ2 ALERT (Gas leak detected)");
    } 
    else if (!dhtError && temperature >= 30.0 && soilDryness >= 70) {
      // HOT & DRY
      currentState = STATE_HOT_DRY;
      Serial.println("Status: HOT & DRY");
    } 
    else if (dhtError || soilDryness >= 55 || (!dhtError && (temperature >= 30.0 || humidity < 40.0))) {
      // WATCH
      currentState = STATE_WATCH;
      Serial.println("Status: WATCH");
    } 
    else {
      // NORMAL
      currentState = STATE_NORMAL;
      Serial.println("Status: NORMAL");
    }

    Serial.println("------------------------------------");
  }

  updateOutputs(currentMillis);
}

void updateOutputs(unsigned long currentMillis) {
  switch (currentState) {
    case STATE_NORMAL:
      turnOffAll();
      break;

    case STATE_WATCH:
      turnOffAll();
      if ((currentMillis / 500) % 2 == 0) {
        digitalWrite(LED_YELLOW, HIGH);
      } else {
        digitalWrite(LED_YELLOW, LOW);
      }
      break;

    case STATE_HOT_DRY:
      turnOffAll();
      digitalWrite(LED_RED, HIGH);
      if ((currentMillis % 1000) < 200) {
        digitalWrite(BUZZER_PIN, LOW);
      } else {
        digitalWrite(BUZZER_PIN, HIGH);
      }
      break;

    case STATE_MQ2_ALERT:
      turnOffAll();
      
      if ((currentMillis / 100) % 2 == 0) {
        digitalWrite(LED_RED, HIGH);
        digitalWrite(BUZZER_PIN, LOW);
      } else {
        digitalWrite(LED_RED, LOW);
        digitalWrite(BUZZER_PIN, HIGH);
      }
      break;
  }
}