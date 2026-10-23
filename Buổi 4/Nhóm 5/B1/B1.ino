#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

#define LED_GREEN  18
#define LED_YELLOW 19
#define LED_RED    21

DHT dht(DHTPIN, DHTTYPE);

void setLEDs(bool g, bool y, bool r) {
  digitalWrite(LED_GREEN, g ? HIGH : LOW);
  digitalWrite(LED_YELLOW, y ? HIGH : LOW);
  digitalWrite(LED_RED, r ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  setLEDs(false, false, false);
  Serial.println("--- Bài 1: Mini Climate Station ---");
}

void loop() {
  delay(2000); // Đọc sau mỗi 2 giây

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Đọc độ C

  // Kiểm tra lỗi cảm biến
  if (isnan(h) || isnan(t)) {
    Serial.println("SENSOR ERROR");
    setLEDs(false, false, false); // Tắt cả 3 LED
    return;
  }

  // Tính Heat Index (false = tính theo độ C)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print(" °C | Do am: ");
  Serial.print(h);
  Serial.print(" % | Heat Index: ");
  Serial.print(hic);
  Serial.print(" °C -> Trang thai: ");

  // Phân loại trạng thái môi trường (mỗi thời điểm chỉ 1 LED sáng)
  if (t >= 31.0) {
    Serial.println("HOT");
    setLEDs(false, false, true); // LED đỏ sáng
  } 
  else if (t < 24.0 || h < 40.0 || h > 75.0) {
    Serial.println("CAUTION");
    setLEDs(false, true, false); // LED vàng sáng
  } 
  else {
    Serial.println("COMFORTABLE");
    setLEDs(true, false, false); // LED xanh sáng
  }
}