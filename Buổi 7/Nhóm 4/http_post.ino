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

    String serverPath = "http://jsonplaceholder.typicode.com/posts";
    http.begin(serverPath); //[cite: 8]

    http.addHeader("Content-Type", "application/json");

    String payload = "{\"temperature\":25.5}"; 
    
    Serial.println("\n--- Bắt đầu gửi dữ liệu ---");
    Serial.println("Payload: " + payload);

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode); 
      
      String response = http.getString();
      Serial.println("Phản hồi từ server: ");
      Serial.println(response);
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