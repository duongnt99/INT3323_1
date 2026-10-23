#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_AO 34

#define LED_GREEN  18
#define LED_YELLOW 19
#define LED_RED    21
#define BUZZER     22

const int ADC_WET = 1200;
const int ADC_DRY = 3800;

DHT dht(DHTPIN, DHTTYPE);

void setOutputs(bool g, bool y, bool r, bool buz) {
  digitalWrite(LED_GREEN, g ? HIGH : LOW);
  digitalWrite(LED_YELLOW, y ? HIGH : LOW);
  digitalWrite(LED_RED, r ? HIGH : LOW);
  digitalWrite(BUZZER, buz ? HIGH : LOW);
}

float calculateDryness(int adcVal) {
  float dryness = (float)(adcVal - ADC_WET) * 100.0 / (ADC_DRY - ADC_WET);
  return constrain(dryness, 0.0, 100.0);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(SOIL_AO, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  setOutputs(false, false, false, false);
  Serial.println("--- Bài 4: Plant Doctor ---");
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilADC = analogRead(SOIL_AO);
  float dryness = calculateDryness(soilADC);

  // Khi DHT11 đọc lỗi, thông báo lỗi thay vì kết luận bình thường
  if (isnan(h) || isnan(t)) {
    Serial.println("[LOI] DHT11 SENSOR ERROR! Khong the danh gia chinh xac tinh trang cay.");
    setOutputs(false, false, false, false);
    return;
  }

  Serial.print("Nhiet do: ");
  Serial.print(t, 1);
  Serial.print("°C | Do am KK: ");
  Serial.print(h, 1);
  Serial.print("% | Do kho dat: ");
  Serial.print(dryness, 1);
  Serial.print("% -> ");

  // 1. Kiểm tra URGENT (Ưu tiên cao nhất)
  if (dryness >= 70.0 && t >= 30.0) {
    Serial.println("Trang thai: URGENT | Ly do: Dat rat kho (>=70%) VA Nhiet do cao (>=30°C)");
    setOutputs(false, false, true, true); // LED đỏ + Buzzer
  }
  // 2. Kiểm tra NEEDS ATTENTION
  else if (dryness >= 55.0 || t >= 30.0 || h < 40.0) {
    Serial.print("Trang thai: NEEDS ATTENTION | Ly do: ");
    if (dryness >= 55.0) Serial.print("[Dat bat dau kho >=55%] ");
    if (t >= 30.0)       Serial.print("[Nhiet do cao >=30°C] ");
    if (h < 40.0)        Serial.print("[Do am khong khi thap <40%] ");
    Serial.println();
    setOutputs(false, true, false, false); // LED vàng
  }
  // 3. Trạng thái NORMAL
  else {
    Serial.println("Trang thai: NORMAL | Moi chi so deu on dinh.");
    setOutputs(true, false, false, false); // LED xanh
  }
}