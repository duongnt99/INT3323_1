#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define PIN_DHT  26
#define DHT_TYPE DHT11

const char* WIFI_SSID = "UET-Wifi-T3 2.4GHz";
const char* WIFI_PASS = "";

const char* MQTT_BROKER = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

const char* TOPIC_TEMP  = "iot/lab7/Nhom3/temperature";
const char* TOPIC_HUMID = "iot/lab7/Nhom3/humidity";

DHT dht(PIN_DHT, DHT_TYPE);
WiFiClient espClient;
PubSubClient mqtt(espClient);

unsigned long lastPublish = 0;
unsigned long lastMqttTry = 0;
unsigned long lastWiFiTry = 0;

void connectMQTT() {
  String clientId = "esp32-sensor-" + String((uint32_t)(ESP.getEfuseMac() & 0xFFFFFF), HEX);
  if (mqtt.connect(clientId.c_str())) {
    Serial.println("MQTT connected (Sensor node).");
  } else {
    Serial.printf("MQTT connect failed, state: %d\n", mqtt.state());
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
}

void loop() {
  unsigned long now = millis();

  if (WiFi.status() != WL_CONNECTED) {
    if (now - lastWiFiTry >= 10000) {
      lastWiFiTry = now;
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
    return;
  }

  if (!mqtt.connected()) {
    if (now - lastMqttTry >= 5000) {
      lastMqttTry = now;
      connectMQTT();
    }
    return;
  }

  mqtt.loop();

  if (now - lastPublish >= 5000) {
    lastPublish = now;

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      mqtt.publish(TOPIC_TEMP, String(t, 1).c_str());
      mqtt.publish(TOPIC_HUMID, String(h, 1).c_str());
      Serial.printf("Published -> Nhiệt độ: %.1f *C | Độ ẩm: %.1f %%\n", t, h);
    } else {
      Serial.println("Lỗi đọc từ cảm biến DHT!");
    }
  }
}