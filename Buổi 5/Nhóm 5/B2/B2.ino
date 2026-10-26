#define SOIL_AO 34
#define SOIL_DO 25

#define LED_GREEN  18
#define LED_YELLOW 19
#define LED_RED    21
#define BUZZER     22

// Giá trị hiệu chuẩn thực tế (Thay đổi sau khi bạn tự đo trên Serial Monitor)
// Thông thường cảm biến điện trở: Nhúng nước (rất ẩm) ADC thấp, để khô ngoài không khí ADC cao
const int ADC_WET = 1200; // Giá trị ADC khi đất rất ẩm (0% khô)
const int ADC_DRY = 3800; // Giá trị ADC khi đất khô hoàn toàn (100% khô)

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
  
  pinMode(SOIL_AO, INPUT);
  pinMode(SOIL_DO, INPUT);
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  setOutputs(false, false, false, false);
  Serial.println("--- Bài 2: Smart Plant Pot ---");
}

void loop() {
  int aoVal = analogRead(SOIL_AO);
  int doVal = digitalRead(SOIL_DO);
  float dryness = calculateDryness(aoVal);

  Serial.print("ADC (AO): ");
  Serial.print(aoVal);
  Serial.print(" | DO: ");
  Serial.print(doVal);
  Serial.print(" | Do kho: ");
  Serial.print(dryness, 1);
  Serial.print("% -> Trang thai: ");

  if (dryness < 35.0) {
    Serial.println("WET");
    setOutputs(true, false, false, false); // LED xanh sáng
  } 
  else if (dryness < 70.0) {
    Serial.println("MODERATE");
    setOutputs(false, true, false, false); // LED vàng sáng
  } 
  else {
    Serial.println("DRY");
    setOutputs(false, false, true, true);  // LED đỏ sáng + Buzzer kêu
  }

  delay(1000); // Cập nhật mỗi giây
}