#define MQ2_AO 35
#define MQ2_DO 26

#define LED_GREEN  18
#define LED_YELLOW 19
#define LED_RED    21
#define BUZZER     22

// Thời gian làm nóng cảm biến (đặt 10 giây khi thực hành, có thể tăng lên 20-30s)
#define WARMUP_SECONDS 10

int baseline = 0;

void setOutputs(bool g, bool y, bool r, bool buz) {
  digitalWrite(LED_GREEN, g ? HIGH : LOW);
  digitalWrite(LED_YELLOW, y ? HIGH : LOW);
  digitalWrite(LED_RED, r ? HIGH : LOW);
  digitalWrite(BUZZER, buz ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(MQ2_AO, INPUT);
  pinMode(MQ2_DO, INPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  setOutputs(false, false, false, false);
  Serial.println("--- Bài 3: Gas Detective ---");
  
  // 1. Chờ cảm biến ổn định
  Serial.println("Dang lam nong cam bien MQ2...");
  for (int i = WARMUP_SECONDS; i > 0; i--) {
    Serial.print("Con lai: ");
    Serial.print(i);
    Serial.println(" giay");
    delay(1000);
  }

  // 2. Đo và tính trung bình 20 mẫu để lấy baseline chính xác
  Serial.println("Dang do gia tri tham chieu (baseline)...");
  long sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += analogRead(MQ2_AO);
    delay(100);
  }
  baseline = sum / 20;
  Serial.print("Baseline da ghi nhan: ");
  Serial.println(baseline);
  Serial.println("----------------------------------------");
}

void loop() {
  int aoVal = analogRead(MQ2_AO);
  int doVal = digitalRead(MQ2_DO);
  int delta = aoVal - baseline;

  Serial.print("MQ2 AO: ");
  Serial.print(aoVal);
  Serial.print(" | MQ2 DO: ");
  Serial.print(doVal);
  Serial.print(" | Baseline: ");
  Serial.print(baseline);
  Serial.print(" | Tang (Delta): ");
  Serial.print(delta);
  Serial.print(" -> Trang thai: ");

  // Phân loại tín hiệu so với baseline
  if (delta < 150) {
    Serial.println("NORMAL");
    setOutputs(true, false, false, false); // LED xanh
  } 
  else if (delta < 400) {
    Serial.println("WARNING");
    setOutputs(false, true, false, false); // LED vàng
  } 
  else {
    Serial.println("ALERT");
    setOutputs(false, false, true, true);  // LED đỏ + Buzzer
  }

  delay(1000);
}