#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Cấu hình chân kết nối phần cứng (ESP32)
#define PIN_LED         23
#define PIN_DHT         26
#define PIN_SOIL_AO     35
#define PIN_MQ2_AO      34
#define PIN_MQ2_DO      19
#define DHT_TYPE        DHT11

// Thông tin kết nối Wi-Fi (2.4 GHz)
const char* WIFI_SSID = "Thinh_WiFi_2.4G";
const char* WIFI_PASS = "123456789";

// Cấu hình MQTT Broker (HiveMQ Public Broker)
const char* MQTT_BROKER = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

// Hậu tố các Topic MQTT cho bài thực hành Buổi 7
const char* SUFFIX_TELEMETRY = "/iot/lab7/telemetry";
const char* SUFFIX_LED_CTRL  = "/iot/lab7/led_control";

DHT dht(PIN_DHT, DHT_TYPE);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

String clientId;
String topicTelemetry;
String topicLedControl;

// Các biến thời gian phục vụ xử lý bất đồng bộ (non-blocking) bằng millis()
unsigned long lastTelemetryTime = 0;
unsigned long lastMqttRetryTime = 0;
unsigned long lastWifiRetryTime = 0;

const unsigned long TELEMETRY_INTERVAL = 5000;
const unsigned long MQTT_RETRY_INTERVAL = 5000;
const unsigned long WIFI_RETRY_INTERVAL = 10000;

struct SensorReadings {
  float temperature;
  float humidity;
  bool dhtValid;
  int soilAnalog;
  int gasAnalog;
  int gasDigital;
};

// Đọc toàn bộ dữ liệu từ các cảm biến
SensorReadings readAllSensors() {
  SensorReadings data;
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();
  data.dhtValid = !(isnan(data.temperature) || isnan(data.humidity));

  data.soilAnalog = analogRead(PIN_SOIL_AO);
  data.gasAnalog  = analogRead(PIN_MQ2_AO);
  data.gasDigital = digitalRead(PIN_MQ2_DO);

  return data;
}

// Điều khiển trạng thái đèn LED
void setLedState(bool turnOn) {
  digitalWrite(PIN_LED, turnOn ? HIGH : LOW);
}

// Hàm callback xử lý bản tin nhận được từ MQTT Broker
void handleMqttMessage(char* topic, byte* payload, unsigned int length) {
  if (String(topic) != topicLedControl) {
    return;
  }

  if (length > 20) {
    Serial.println("Command too long.");
    return;
  }

  String cmd = "";
  cmd.reserve(length);
  for (unsigned int i = 0; i < length; i++) {
    cmd += (char)payload[i];
  }

  cmd.trim();
  cmd.toUpperCase();

  Serial.print("Received command: ");
  Serial.println(cmd);

  if (cmd == "ON") {
    setLedState(true);
    Serial.println("LED ON");
  } else if (cmd == "OFF") {
    setLedState(false);
    Serial.println("LED OFF");
  } else {
    Serial.println("Unknown command.");
  }
}

// Kết nối tới MQTT Broker và đăng ký (Subscribe) topic điều khiển LED
void reconnectMqtt() {
  uint32_t macSuffix = (uint32_t)(ESP.getEfuseMac() & 0xFFFFFF);
  clientId = "esp32-" + String(macSuffix, HEX);

  if (mqttClient.connect(clientId.c_str())) {
    Serial.println("MQTT connected.");

    topicLedControl = clientId + SUFFIX_LED_CTRL;
    topicTelemetry  = clientId + SUFFIX_TELEMETRY;

    mqttClient.subscribe(topicLedControl.c_str());
    Serial.printf("Subscribed to: %s\n", topicLedControl.c_str());
  } else {
    Serial.print("MQTT failed: ");
    Serial.println(mqttClient.state());
  }
}

// Đóng gói JSON và gửi (Publish) dữ liệu cảm biến lên MQTT Broker
void publishSensorTelemetry(const SensorReadings& data) {
  JsonDocument doc;

  doc["temperature"] = serialized(String(data.temperature, 1));
  doc["humidity"]    = serialized(String(data.humidity, 1));
  doc["soilAnalog"]  = data.soilAnalog;
  doc["gasAnalog"]   = data.gasAnalog;
  doc["gasDigital"]  = data.gasDigital;

  JsonObject quality = doc["quality"].to<JsonObject>();
  quality["dht"] = data.dhtValid ? "valid" : "invalid";

  String jsonPayload;
  serializeJsonPretty(doc, jsonPayload);

  mqttClient.publish(topicTelemetry.c_str(), jsonPayload.c_str());
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Khởi tạo các chân I/O và cảm biến
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_MQ2_DO, INPUT);
  setLedState(true);

  analogReadResolution(12);
  dht.begin();

  // Kết nối Wi-Fi ở chế độ Station
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - wifiStart < 20000)) {
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wi-Fi IP: ");
    Serial.println(WiFi.localIP());
  }

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(handleMqttMessage);
}

void loop() {
  unsigned long currentMillis = millis();

  // Kiểm tra và tự động kết nối lại Wi-Fi nếu bị mất kết nối
  if (WiFi.status() != WL_CONNECTED) {
    if (currentMillis - lastWifiRetryTime >= WIFI_RETRY_INTERVAL) {
      lastWifiRetryTime = currentMillis;
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
    delay(10);
    return;
  }

  // Kiểm tra và tự động kết nối lại MQTT Broker
  if (!mqttClient.connected()) {
    if (lastMqttRetryTime == 0 || (currentMillis - lastMqttRetryTime >= MQTT_RETRY_INTERVAL)) {
      lastMqttRetryTime = currentMillis;
      reconnectMqtt();
    }
    delay(10);
    return;
  }

  mqttClient.loop();

  // Định kỳ đọc cảm biến và Publish dữ liệu mỗi 5 giây
  if (currentMillis - lastTelemetryTime >= TELEMETRY_INTERVAL) {
    lastTelemetryTime = currentMillis;
    SensorReadings readings = readAllSensors();
    publishSensorTelemetry(readings);
  }
}
