#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi có sẵn
const char* STA_SSID = "TEN_WIFI";
const char* STA_PASSWORD = "MAT_KHAU_WIFI";

// Mạng ESP32 
const char* AP_SSID = "ESP32_Combo";
const char* AP_PASSWORD = "ComboESP32_2026";

IPAddress apIP(192, 168, 50, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

void handleHome() {
  String html = "<h2>ESP32 AP + STA</h2>";

  html += "<p>AP IP: ";
  html += WiFi.softAPIP().toString();
  html += "</p>";

  html += "<p>Station IP: ";
  html += WiFi.localIP().toString();
  html += "</p>";

  html += "<p>STA Connected: ";
  html += WiFi.status() == WL_CONNECTED ?
          "YES" : "NO";
  html += "</p>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Kích hoạt khả năng STA + AP
  WiFi.mode(WIFI_AP_STA);

  Serial.println("Connecting to external Wi-Fi...");

  WiFi.begin(STA_SSID, STA_PASSWORD);

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startTime < 20000) {

    delay(500);
    Serial.print(".");
  }

  // Theo yêu cầu của lab:
  // STA phải kết nối trước khi bật AP
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nSTA connection failed.");
    Serial.println("Check Wi-Fi and restart.");
    return;
  }

  Serial.println("\nSTA connected!");

  Serial.print("Station IP: ");
  Serial.println(WiFi.localIP());

  // Cấu hình mạng AP riêng
  WiFi.softAPConfig(apIP, apIP, subnet);

  bool apOK = WiFi.softAP(
    AP_SSID, AP_PASSWORD
  );

  if (!apOK) {
    Serial.println("AP startup failed");
    return;
  }

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleHome);
  server.begin();

  Serial.println("Combination mode ready!");
}

void loop() {
  server.handleClient();
}