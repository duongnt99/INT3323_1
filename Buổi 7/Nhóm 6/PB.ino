#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Điền thông tin Wi-Fi 2.4 GHz của bạn.
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

const char* MQTT_HOST = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;
const char* TOPIC_TELEMETRY = "iot/lab7/group6/telemetry";
const char* TOPIC_LED_CONTROL = "iot/lab7/group6/led_control";

constexpr uint8_t LED_PIN = 23;
constexpr uint8_t DHT_PIN = 26;
constexpr uint8_t SOIL_PIN = 35;
constexpr uint8_t MQ2_ANALOG_PIN = 34;
constexpr uint8_t MQ2_DIGITAL_PIN = 19;
constexpr uint8_t DHT_TYPE = DHT11;

// Hiệu chỉnh các ngưỡng này theo cảm biến thực tế.
constexpr float HIGH_TEMPERATURE_C = 35.0f;
constexpr int DRY_SOIL_THRESHOLD = 2800; // ADC cao thường là đất khô
constexpr int HIGH_GAS_THRESHOLD = 2000;
constexpr unsigned long TELEMETRY_INTERVAL_MS = 5000;
constexpr unsigned long WIFI_RETRY_INTERVAL_MS = 10000;
constexpr unsigned long MQTT_RETRY_INTERVAL_MS = 5000;

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastTelemetry = 0;
unsigned long lastWiFiRetry = 0;
unsigned long lastMqttRetry = 0;
bool automaticLedMode = true;
bool manualLedState = false;

void applyLedState(bool alarmActive) {
  const bool ledOn = automaticLedMode ? alarmActive : manualLedState;
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  if (String(topic) != TOPIC_LED_CONTROL || length > 20) {
    return;
  }

  String command;
  for (unsigned int i = 0; i < length; ++i) {
    command += static_cast<char>(payload[i]);
  }
  command.trim();
  command.toUpperCase();

  if (command == "AUTO") {
    automaticLedMode = true;
    Serial.println("LED mode: automatic");
  } else if (command == "ON") {
    automaticLedMode = false;
    manualLedState = true;
    Serial.println("LED: ON (manual)");
  } else if (command == "OFF") {
    automaticLedMode = false;
    manualLedState = false;
    Serial.println("LED: OFF (manual)");
  } else {
    Serial.println("Unknown command. Use ON, OFF, or AUTO.");
  }

  // Chế độ tự động sẽ cập nhật LED ở lần đọc cảm biến kế tiếp.
  if (!automaticLedMode) {
    applyLedState(false);
  }
}

void connectMqtt() {
  const String clientId = "esp32-group6-" +
      String(static_cast<uint32_t>(ESP.getEfuseMac() & 0xFFFFFF), HEX);

  if (mqtt.connect(clientId.c_str())) {
    Serial.println("MQTT connected.");
    mqtt.subscribe(TOPIC_LED_CONTROL);
    Serial.print("Subscribed: ");
    Serial.println(TOPIC_LED_CONTROL);
  } else {
    Serial.print("MQTT connection failed, state: ");
    Serial.println(mqtt.state());
  }
}

void publishTelemetry() {
  const float temperature = dht.readTemperature();
  const float humidity = dht.readHumidity();
  const bool dhtValid = !isnan(temperature) && !isnan(humidity);
  const int soil = analogRead(SOIL_PIN);
  const int gasAnalog = analogRead(MQ2_ANALOG_PIN);
  const int gasDigital = digitalRead(MQ2_DIGITAL_PIN);

  const bool highTemperature = dhtValid && temperature >= HIGH_TEMPERATURE_C;
  const bool drySoil = soil >= DRY_SOIL_THRESHOLD;
  const bool highGas = gasAnalog >= HIGH_GAS_THRESHOLD || gasDigital == LOW;
  const bool alarmActive = highTemperature || drySoil || highGas;

  applyLedState(alarmActive);

  JsonDocument doc;
  if (dhtValid) {
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
  } else {
    doc["temperature"] = nullptr;
    doc["humidity"] = nullptr;
  }
  doc["soilAnalog"] = soil;
  doc["gasAnalog"] = gasAnalog;
  doc["gasDigital"] = gasDigital;
  doc["alert"]["highTemperature"] = highTemperature;
  doc["alert"]["drySoil"] = drySoil;
  doc["alert"]["highGas"] = highGas;
  doc["alert"]["active"] = alarmActive;
  doc["quality"]["dht"] = dhtValid ? "valid" : "invalid";

  String payload;
  serializeJson(doc, payload);
  const bool sent = mqtt.publish(TOPIC_TELEMETRY, payload.c_str());

  Serial.print("Telemetry: ");
  Serial.println(payload);
  Serial.println(sent ? "Publish accepted." : "Publish failed.");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MQ2_DIGITAL_PIN, INPUT);
  analogReadResolution(12);
  dht.begin();
  applyLedState(false);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);
}

void loop() {
  const unsigned long now = millis();

  if (WiFi.status() != WL_CONNECTED) {
    if (now - lastWiFiRetry >= WIFI_RETRY_INTERVAL_MS) {
      lastWiFiRetry = now;
      Serial.println("Reconnecting to Wi-Fi...");
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
    delay(10);
    return;
  }

  if (!mqtt.connected()) {
    if (now - lastMqttRetry >= MQTT_RETRY_INTERVAL_MS) {
      lastMqttRetry = now;
      connectMqtt();
    }
    delay(10);
    return;
  }

  mqtt.loop();

  if (now - lastTelemetry >= TELEMETRY_INTERVAL_MS) {
    lastTelemetry = now;
    publishTelemetry();
  }
}
