#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Group4_MTTQ";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* topic_led_control = "iot/lab5/htzon/led_control";

const int ledPin = 2; 

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Đã nhận tin nhắn từ topic [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String messageTemp = "";
  for (int i = 0; i < length; i++) {
    messageTemp += (char)payload[i];
  }
  Serial.println(messageTemp);

  if (messageTemp == "ON") {
    digitalWrite(ledPin, HIGH);
    Serial.println("-> Đã BẬT đèn LED");
  } else if (messageTemp == "OFF") {
    digitalWrite(ledPin, LOW); 
    Serial.println("-> Đã TẮT đèn LED");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-Sub-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(topic_led_control);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
  
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  client.setServer(mqtt_server, mqtt_port);
  
  client.setCallback(callback); 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 