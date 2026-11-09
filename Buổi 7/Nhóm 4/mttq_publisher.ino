#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Group4_MTTQ";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* topic_temperature = "iot/lab5/htzon/temperature";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  client.setServer(mqtt_server, mqtt_port);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    float temp = random(200, 350) / 10.0;
    String payload = String(temp); 
    
    Serial.print("Publish message: ");
    Serial.println(payload);
    
    client.publish(topic_temperature, payload.c_str());
  }
}