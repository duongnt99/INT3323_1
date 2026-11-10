
#include <WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "Vanhh🙈";
const char* WIFI_PASS = "nguyenthivananh";

const char* MQTT_HOST = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

const int LED_PIN = 23;

const char* TOPIC_TEMP =
  "iot/lab7/nhom1_vananh/temperature";

const char* TOPIC_LED = "iot/lab7/nhom1_vananh/led_control";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastPublish = 0;
unsigned long lastMqttTry = 0;
unsigned long lastWiFiTry = 0;

// Ham xu ly khi nhan thong diep MQTT.
void onMessage(
  char* topic,
  byte* payload,
  unsigned int length
) {
  if (String(topic) != TOPIC_LED) return;

  if (length > 20) {
    Serial.println("Command too long.");
    return;
  }

  String command = "";

  for (unsigned int i = 0; i < length; i++) {
    command += (char)payload[i];
  }

  command.trim();
  command.toUpperCase();

  Serial.print("Received command: ");
  Serial.println(command);

  if (command == "ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
  }
  else if (command == "OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
  }
  else {
    Serial.println("Unknown command.");
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

    mqtt.subscribe(TOPIC_LED);

    Serial.print("Subscribed: ");
    Serial.println(TOPIC_LED);
  }
  else {
    Serial.print("MQTT failed: ");
    Serial.println(mqtt.state());
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  randomSeed(micros());

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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
  }

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMessage);
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

  // Van tiep tuc gui nhiet do.
  if (now - lastPublish >= 5000) {
    lastPublish = now;

    float temperature =
      random(250, 351) / 10.0f;

    String message = String(temperature, 1);

    mqtt.publish(
      TOPIC_TEMP,
      message.c_str()
    );

    Serial.print("Temperature: ");
    Serial.println(message);
  }
}
