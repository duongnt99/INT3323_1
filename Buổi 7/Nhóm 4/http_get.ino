#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Group4";
const char* password = "";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = "http://jsonplaceholder.typicode.com/users/1";
    http.begin(serverPath);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode); 
      
      String payload = http.getString();
      Serial.println("Payload: ");
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode); 
    }
    
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(10000); 
}
