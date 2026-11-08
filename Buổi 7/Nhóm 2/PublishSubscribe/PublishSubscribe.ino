#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <IoTNode.h>

const char* WIFI_SSID = "P2011_2.4GHz";
const char* WIFI_PASS = "lmaolmao123";

const char* MQTT_HOST = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

const char* TOPIC_TELEMETRY = "/iot/lab7/telemetry";
const char* TOPIC_LED = "/iot/lab7/led_control";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastPublish = 0;
unsigned long lastMqttTry = 0;
unsigned long lastWiFiTry = 0;

String topicLed;
String topicTemperature;

void onMessage(char* topic, byte* payload, unsigned int length)
{
  if (String(topic) != topicLed)
  {
    return;
  }

  if (length > 20)
  {
    Serial.println("Command too long.");
    return;
  }

  String command = "";

  for (unsigned int i = 0; i < length; i++) 
  {
    command += (char)payload[i];
  }

  command.trim();
  command.toUpperCase();

  Serial.print("Received command: ");
  Serial.println(command);

  if (command == "ON") 
  {
    iotNode.setLed(true);
    Serial.println("LED ON");
  }
  else if (command == "OFF") 
  {
    iotNode.setLed(false);
    Serial.println("LED OFF");
  }
  else 
  {
    Serial.println("Unknown command.");
  }
}

void connectMQTT() {
  String clientId = "esp32-" +
    String(
      (uint32_t)(ESP.getEfuseMac() & 0xFFFFFF),
      HEX
    );

  if (mqtt.connect(clientId.c_str())) 
  {
    Serial.println("MQTT connected.");

    topicLed = clientId + TOPIC_LED;
    topicTemperature = clientId + TOPIC_TELEMETRY;

    mqtt.subscribe(topicLed.c_str());
    Serial.printf("Subscribed to: %s\n", topicLed.c_str());
  }
  else 
  {
    Serial.print("MQTT failed: ");
    Serial.println(mqtt.state());
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  iotNode.setUp();

  randomSeed(micros());

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

  if (!mqtt.connected()) 
  {
    if (lastMqttTry == 0 || now - lastMqttTry >= 5000) 
    {
      lastMqttTry = now;
      connectMQTT();
    }
    delay(10);
    return;
  }

  mqtt.loop();

  if (now - lastPublish >= 5000) 
  {
    Telemetry telemetry = iotNode.getTelemetry();
    publishTelemetry(telemetry);
  }
}

void publishTelemetry(Telemetry telemetry)
{
  lastPublish = millis();

  JsonDocument doc;
  doc["temperature"] = telemetry.temperature;
  doc["humidity"] = telemetry.temperature;
  doc["soilAnalog"] = telemetry.soilAnalog;
  doc["gasAnalog"] = telemetry.gasAnalog;
  doc["gasDigital"] = telemetry.gasDigital;

  JsonObject quality = doc["quality"].to<JsonObject>();
  quality["dht"] = telemetry.trustedTemperatureHumidity ? "valid" : "invalid";

  String message;
  serializeJsonPretty(doc, message);

  mqtt.publish(
    topicTemperature.c_str(),
    message.c_str()
  );
}