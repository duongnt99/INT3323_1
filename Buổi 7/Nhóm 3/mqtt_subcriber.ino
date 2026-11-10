#include <WiFi.h>
#include <PubSubClient.h>

#define LED 23

const char* WIFI_SSID = "UET-Wifi-T3 2.4GHz";
const char* WIFI_PASS = "";

const char* MQTT_BROKER = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

const char* TOPIC_LED_CTRL = "iot/lab7/Nhom3/led_control";

WiFiClient espClient;
PubSubClient mqtt(espClient);

unsigned long lastMqttTry = 0;
unsigned long lastWiFiTry = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  String cmd = "";
  for (unsigned int i = 0; i < length; i++) {
    cmd += (char)payload[i];
  }
  cmd.trim();

  Serial.printf("Nhan lenh [%s]: %s\n", topic, cmd.c_str());

  if (cmd.equalsIgnoreCase("ON")) {
    digitalWrite(LED, HIGH);
    Serial.println("Trang thai LED: BAT");
  } else if (cmd.equalsIgnoreCase("OFF")) {
    digitalWrite(LED, LOW);
    Serial.println("Trang thai LED: TAT");
  }
}

void connectMQTT() {
  String clientId = "esp32-actuator-" + String((uint32_t)(ESP.getEfuseMac() & 0xFFFFFF), HEX);
  if (mqtt.connect(clientId.c_str())) {
    Serial.println("MQTT connected (LED node).");
    mqtt.subscribe(TOPIC_LED_CTRL);
    Serial.printf("Subscribed: %s\n", TOPIC_LED_CTRL);
  } else {
    Serial.printf("MQTT connect failed, state: %d\n", mqtt.state());
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW); 

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(callback);
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
}