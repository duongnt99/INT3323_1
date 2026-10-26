#include <WiFi.h>


const char* WIFI_SSID = "TEN_WIFI";
const char* WIFI_PASSWORD = "MAT_KHAU_WIFI";

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n=== ESP32 STATION MODE ===");

  // Chỉ hoạt động ở chế độ Station
  WiFi.mode(WIFI_STA);

  // Bắt đầu kết nối
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting");

  unsigned long startTime = millis();

  // Chờ tối đa 20 giây
  while (WiFi.status() != WL_CONNECTED &&
         millis() - startTime < 20000) {

    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");

    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  }
  else {
    Serial.println("\nConnection failed!");
  }
}

void loop() {
  // Kiểm tra trạng thái mỗi 5 giây
  static unsigned long lastCheck = 0;

  if (millis() - lastCheck >= 5000) {
    lastCheck = millis();

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Wi-Fi is connected");
    }
    else {
      Serial.println("Wi-Fi disconnected");
      Serial.println("Retrying...");

      WiFi.reconnect();
    }
  }
}