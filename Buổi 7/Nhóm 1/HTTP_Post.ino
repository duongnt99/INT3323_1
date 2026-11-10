
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char* WIFI_SSID = "Vanhh🙈";
const char* WIFI_PASS = "nguyenthivananh";

const char* API_URL =
  "http://jsonplaceholder.typicode.com/posts";

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting to Wi-Fi");

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - start < 20000) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wi-Fi IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wi-Fi failed.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  randomSeed(micros());
  connectWiFi();

  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  // Gia lap nhiet do 25.0 - 35.0 do C.
  float temperature =
    random(250, 351) / 10.0f;

  // Tao JSON de gui len may chu.
  String json =
    "{\"userId\":1,\"title\":\"ESP32 Lab 7\","
    "\"body\":\"Simulated temperature\","
    "\"temperature\":";

  json += String(temperature, 1);
  json += "}";

  WiFiClient client; // Chi dung demo.

  HTTPClient http;
  http.setTimeout(12000);

  Serial.println("Sending HTTP POST...");
  Serial.println(json);

  if (!http.begin(client, API_URL)) {
    Serial.println("HTTP begin failed.");
    return;
  }

  http.addHeader(
    "Content-Type",
    "application/json"
  );

  int httpCode = http.POST(json);

  Serial.print("HTTP response code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    Serial.println("Server response:");
    Serial.println(http.getString());
  } else {
    Serial.println(
      http.errorToString(httpCode)
    );
  }

  http.end();
}

void loop() {
  // POST duoc thuc hien mot lan.
}
