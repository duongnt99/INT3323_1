#include <WiFi.h>
#include <WebServer.h>

const char* AP_SSID = "ESP32_Lab4";
const char* AP_PASSWORD = "LabESP32_2026";

// Địa chỉ mạng nội bộ ESP32 tự phát
IPAddress localIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

void handleHome() {
  String html = "<html>";
  html += "<head>";
  html += "<meta name='viewport' ";
  html += "content='width=device-width, initial-scale=1'>";
  html += "</head><body>";

  html += "<h2>Hello from ESP32!</h2>";
  html += "<p>Wi-Fi Mode: Access Point</p>";
  html += "<p>IP: ";
  html += WiFi.softAPIP().toString();
  html += "</p>";

  html += "<p>Connected devices: ";
  html += String(WiFi.softAPgetStationNum());
  html += "</p>";

  html += "</body></html>";

  server.send(200, "text/html; charset=utf-8", html);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);

  WiFi.softAPConfig(
    localIP, gateway, subnet
  );

  bool success = WiFi.softAP(
    AP_SSID, AP_PASSWORD
  );

  if (success) {
    Serial.println("ESP32 Access Point started");

    Serial.print("SSID: ");
    Serial.println(AP_SSID);

    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
  }
  else {
    Serial.println("AP startup failed");
  }

  server.on("/", handleHome);
  server.begin();

  Serial.println("HTTP server ready");
}

void loop() {
  server.handleClient();
}