
#include <WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "Vanhh🙈";
const char* WIFI_PASS = "nguyenthivananh";

const char* MQTT_HOST = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

const char* TOPIC_TEMP =
  "iot/lab7/Group1/temperature";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastPublish = 0;
unsigned long lastMqttTry = 0;
unsigned long lastWiFiTry = 0;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - start < 20000) {
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wi-Fi IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wi-Fi unavailable.");
  }
}

void connectMQTT() {
  String clientId = "esp32-lab7-" +
    String(
      (uint32_t)(ESP.getEfuseMac() & 0xFFFFFF),
      HEX
    );

  if (mqtt.connect(clientId.c_str())) {
    Serial.println("MQTT connected.");
  } else {
    Serial.print("MQTT failed, state = ");
    Serial.println(mqtt.state());
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  randomSeed(micros());
  connectWiFi();

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
}

void loop() {
  unsigned long now = millis();

  if (WiFi.status() != WL_CONNECTED) {
    if (now - lastWiFiTry >= 10000) {
      lastWiFiTry = now;
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
    delay(10);
    return;
  }

  if (!mqtt.connected()) {
    if (lastMqttTry == 0 ||
        now - lastMqttTry >= 5000) {
      lastMqttTry = now;
      connectMQTT();
    }
    delay(10);
    return;
  }

  mqtt.loop();

  if (now - lastPublish >= 5000) {
    lastPublish = now;

    float temperature =
      random(250, 351) / 10.0f;

    String message = String(temperature, 1);

    bool sent = mqtt.publish(
      TOPIC_TEMP,
      message.c_str()
    );

    Serial.print("Publish temperature: ");
    Serial.print(message);
    Serial.print(" | Accepted: ");
    Serial.println(sent ? "YES" : "NO");
  }
}
