#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_AO 34
#define MQ2_AO  35
#define MQ2_DO  26

#define LED_GREEN  18
#define LED_YELLOW 19
#define LED_RED    21
#define BUZZER     22

// Hằng số hiệu chuẩn độ ẩm đất & ngưỡng MQ2
const int ADC_WET = 1200;
const int ADC_DRY = 3800;
const int MQ2_WATCH_DELTA = 150; // Ngưỡng MQ2 cần chú ý (WATCH)
const int MQ2_ALERT_DELTA = 400; // Ngưỡng MQ2 báo động (MQ2 ALERT)

DHT dht(DHTPIN, DHTTYPE);

// Các trạng thái hoạt động của hệ thống
enum SystemState {
  STATE_NORMAL,
  STATE_WATCH,
  STATE_HOT_DRY,
  STATE_MQ2_ALERT,
  STATE_SENSOR_ERROR
};

SystemState currentState = STATE_NORMAL;
int mq2Baseline = 0;

// Biến thời gian cho millis()
unsigned long lastSensorReadTime = 0;
const unsigned long SENSOR_INTERVAL = 2000; // Lấy mẫu mỗi 2 giây

unsigned long lastBlinkTime = 0;
bool toggleState = false;

float calculateDryness(int adcVal) {
  float dryness = (float)(adcVal - ADC_WET) * 100.0 / (ADC_DRY - ADC_WET);
  return constrain(dryness, 0.0, 100.0);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(SOIL_AO, INPUT);
  pinMode(MQ2_AO, INPUT);
  pinMode(MQ2_DO, INPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.println("--- Bài 5: Greenhouse Guardian ---");
  Serial.println("Dang khoi dong va lay baseline cho MQ2 (5 giay)...");
  
  // Chỉ dùng delay trong setup() để lấy mẫu baseline ban đầu
  delay(3000);
  long sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += analogRead(MQ2_AO);
    delay(100);
  }
  mq2Baseline = sum / 20;
  Serial.print("MQ2 Baseline: ");
  Serial.println(mq2Baseline);
  Serial.println("He thong bat dau giam sat (Non-blocking millis)...");
}

// Hàm đọc cảm biến và cập nhật trạng thái (chạy mỗi 2000ms)
void readSensorsAndEvaluate() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilADC = analogRead(SOIL_AO);
  float dryness = calculateDryness(soilADC);
  int mq2AO = analogRead(MQ2_AO);
  int mq2DO = digitalRead(MQ2_DO);
  int mq2Delta = mq2AO - mq2Baseline;

  bool dhtError = (isnan(h) || isnan(t));

  // In thông số lên Serial Monitor
  if (dhtError) {
    Serial.print("SENSOR ERROR (DHT11) | ");
  } else {
    Serial.print("Temp: "); Serial.print(t, 1); Serial.print("°C | ");
    Serial.print("Hum: ");  Serial.print(h, 1); Serial.print("% | ");
  }
  Serial.print("Soil Dryness: "); Serial.print(dryness, 1); Serial.print("% | ");
  Serial.print("MQ2 AO: "); Serial.print(mq2AO);
  Serial.print(" (Delta: "); Serial.print(mq2Delta); Serial.print(") | ");
  Serial.print("MQ2 DO: "); Serial.print(mq2DO);
  Serial.print(" => Priority State: ");

  // Xét thứ tự ưu tiên: MQ2 ALERT -> HOT & DRY -> WATCH -> NORMAL
  // Lưu ý: Nếu DHT11 lỗi, vẫn kiểm tra MQ2 ALERT hoặc độ khô đất trước, tuyệt đối không vào NORMAL
  if (mq2Delta >= MQ2_ALERT_DELTA) {
    currentState = STATE_MQ2_ALERT;
    Serial.println("MQ2 ALERT");
  } 
  else if (!dhtError && (t >= 30.0 && dryness >= 70.0)) {
    currentState = STATE_HOT_DRY;
    Serial.println("HOT & DRY");
  } 
  else if (dhtError) {
    currentState = STATE_SENSOR_ERROR;
    Serial.println("SENSOR ERROR (Khong hien thi NORMAL)");
  } 
  else if (dryness >= 55.0 || t >= 30.0 || t < 24.0 || h < 40.0 || h > 75.0 || mq2Delta >= MQ2_WATCH_DELTA) {
    currentState = STATE_WATCH;
    Serial.println("WATCH");
  } 
  else {
    currentState = STATE_NORMAL;
    Serial.println("NORMAL");
  }
}

// Hàm điều khiển LED và Buzzer đồng thời bằng millis()
void handleOutputsNonBlocking(unsigned long currentMillis) {
  switch (currentState) {
    case STATE_NORMAL:
      // LED xanh sáng cố định
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(BUZZER, LOW);
      break;

    case STATE_WATCH:
      // LED vàng nhấp nháy (chu kỳ 500ms)
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(BUZZER, LOW);
      if (currentMillis - lastBlinkTime >= 500) {
        lastBlinkTime = currentMillis;
        toggleState = !toggleState;
        digitalWrite(LED_YELLOW, toggleState ? HIGH : LOW);
      }
      break;

    case STATE_HOT_DRY:
      // LED đỏ sáng cố định, Buzzer phát tiếng ngắn định kỳ (kêu 150ms, nghỉ 1350ms)
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, HIGH);
      {
        unsigned long cycleTime = currentMillis % 1500;
        digitalWrite(BUZZER, (cycleTime < 150) ? HIGH : LOW);
      }
      break;

    case STATE_MQ2_ALERT:
      // LED đỏ nhấp nháy nhanh và Buzzer phát tiếng nhanh hơn (chu kỳ 150ms đảo trạng thái)
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
      if (currentMillis - lastBlinkTime >= 150) {
        lastBlinkTime = currentMillis;
        toggleState = !toggleState;
        digitalWrite(LED_RED, toggleState ? HIGH : LOW);
        digitalWrite(BUZZER, toggleState ? HIGH : LOW);
      }
      break;

    case STATE_SENSOR_ERROR:
      // Khi lỗi DHT11: Tắt LED xanh (không hiển thị NORMAL), nháy chậm LED vàng cảnh báo lỗi phần cứng
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(BUZZER, LOW);
      if (currentMillis - lastBlinkTime >= 1000) {
        lastBlinkTime = currentMillis;
        toggleState = !toggleState;
        digitalWrite(LED_YELLOW, toggleState ? HIGH : LOW);
      }
      break;
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Lấy mẫu cảm biến định kỳ bằng millis()
  if (currentMillis - lastSensorReadTime >= SENSOR_INTERVAL) {
    lastSensorReadTime = currentMillis;
    readSensorsAndEvaluate();
  }

  // 2. Điều khiển LED và Buzzer liên tục không dùng delay()
  handleOutputsNonBlocking(currentMillis);
}