#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* WIFI_SSID = "UET-Wifi-T3 2.4Ghz";
const char* WIFI_PASS = "";

const char* MQTT_HOST = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

const char* TOPIC_TEMP  = "iot/lab7/Group7/temperature";
const char* TOPIC_HUMID = "iot/lab7/Group7/humidity";
const char* TOPIC_GAS   = "iot/lab7/Group7/gas";
const char* TOPIC_SOIL  = "iot/lab7/Group7/soil";

#define DHTPIN 4        
#define DHTTYPE DHT11   
#define MQ2_PIN 34      
#define SOIL_PIN 35     
DHT dht(DHTPIN, DHTTYPE);

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastPublish = 0;
unsigned long lastMqttTry = 0;
unsigned long lastWiFiTry = 0;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
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
  String clientId = "esp32-lab7-" + String((uint32_t)(ESP.getEfuseMac() & 0xFFFFFF), HEX);
  if (mqtt.connect(clientId.c_str())) {
    Serial.println("MQTT connected.");
  } else {
    Serial.print("MQTT failed, state = ");
    Serial.println(mqtt.state());
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  // Khởi tạo cảm biến DHT
  dht.begin();
  
  // Khởi tạo các chân Analog
  pinMode(MQ2_PIN, INPUT);
  pinMode(SOIL_PIN, INPUT);

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
    if (lastMqttTry == 0 || now - lastMqttTry >= 5000) {
      lastMqttTry = now;
      connectMQTT();
    }
    delay(10);
    return;
  }

  mqtt.loop();

  if (now - lastPublish >= 5000) {
    lastPublish = now;

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int gasValue = analogRead(MQ2_PIN);
    int soilValue = analogRead(SOIL_PIN);

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    String msgTemp = String(temperature, 1);
    String msgHumid = String(humidity, 1);
    String msgGas = String(gasValue);
    String msgSoil = String(soilValue);

    bool sentTemp  = mqtt.publish(TOPIC_TEMP, msgTemp.c_str());
    bool sentHumid = mqtt.publish(TOPIC_HUMID, msgHumid.c_str());
    bool sentGas   = mqtt.publish(TOPIC_GAS, msgGas.c_str());
    bool sentSoil  = mqtt.publish(TOPIC_SOIL, msgSoil.c_str());

    Serial.println("--- Gửi dữ liệu Cảm biến ---");
    Serial.printf("Temp: %s *C | Accepted: %s\n", msgTemp.c_str(), sentTemp ? "YES" : "NO");
    Serial.printf("Humid: %s %% | Accepted: %s\n", msgHumid.c_str(), sentHumid ? "YES" : "NO");
    Serial.printf("Gas MQ2: %s | Accepted: %s\n", msgGas.c_str(), sentGas ? "YES" : "NO");
    Serial.printf("Soil: %s | Accepted: %s\n", msgSoil.c_str(), sentSoil ? "YES" : "NO");
    Serial.println("----------------------------");
  }
}