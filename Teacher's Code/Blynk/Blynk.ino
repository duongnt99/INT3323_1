
#define BLYNK_TEMPLATE_ID "TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "AUTH_TOKEN_MOI"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Thông tin Wi-Fi 2.4 GHz
char ssid[] = "TEN_WIFI";
char pass[] = "MAT_KHAU_WIFI";

// LED vật lý
const int LED_PIN = 25;

BlynkTimer timer;

// Gửi dữ liệu giả lập lên Dashboard
void sendData() {
  int gasValue = random(1000, 5001);

  float temperature =
      random(240, 341) / 10.0;

  float humidity =
      random(450, 801) / 10.0;

  Blynk.virtualWrite(V1, gasValue);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);

  Serial.print("Gas: ");
  Serial.println(gasValue);

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);
}

// Nhận lệnh từ Switch V0
BLYNK_WRITE(V0) {
  int ledState = param.asInt();

  digitalWrite(
    LED_PIN,
    ledState == 1 ? HIGH : LOW
  );

  Serial.println(
    ledState == 1 ? "LED ON" : "LED OFF"
  );
}

// Khi kết nối Blynk thành công
BLYNK_CONNECTED() {
  Serial.println("Blynk connected!");

  // Đồng bộ trạng thái Switch V0
  Blynk.syncVirtual(V0);

  // Gửi dữ liệu ngay lần đầu kết nối
  sendData();
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  randomSeed(micros());

  // Cập nhật định kỳ mỗi 120 giây
  timer.setInterval(120000L, sendData);

  // Kết nối Wi-Fi và Blynk Cloud
  Blynk.begin(
    BLYNK_AUTH_TOKEN,
    ssid,
    pass
  );
}

void loop() {
  Blynk.run();
  timer.run();
}
