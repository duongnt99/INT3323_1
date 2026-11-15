#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char* WIFI_SSID = "Vanhh🙈";        // Điền tên hotspot, GIỮ ngoặc kép
const char* WIFI_PASS = "nguyenthivananh";   // Điền mật khẩu, GIỮ ngoặc kép

const char* API_URL = "http://jsonplaceholder.typicode.com/users/1";

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to Wi-Fi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wi-Fi connected. ESP32 IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wi-Fi connection failed.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  connectWiFi();
  if (WiFi.status() != WL_CONNECTED) return;   // Không có WiFi thì dừng

  WiFiClient client;       // Bỏ qua kiểm tra chứng chỉ (chỉ dùng cho bài lab)

  HTTPClient http;
  http.setTimeout(12000);
  Serial.println("Sending HTTP GET...");

  if (!http.begin(client, API_URL)) {
    Serial.println("HTTP begin failed.");
    return;
  }

  int httpCode = http.GET();
  Serial.print("HTTP response code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    Serial.println("Response payload:");
    Serial.println(http.getString());
  } else {
    Serial.println(http.errorToString(httpCode));
  }
  http.end();
}

void loop() {
  // GET chỉ thực hiện một lần trong setup()
}