const int MQ_AO = 34;
const int MQ_DO = 18;

const int LED_GREEN = 23;
const int LED_YELLOW = 22;
const int LED_RED = 25;
const int BUZZER_PIN = 32;

const int THRESHOLD_WARN = 150;
const int THRESHOLD_ALERT = 400;

int baseline = 0;

void turnOffAll(){
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, HIGH);
}

void setup() {
  Serial.begin(9600);

  pinMode(MQ_DO, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  turnOffAll();

  analogReadResolution(12);
  Serial.println("MQ2 Sensor warming up...");
  delay(5000);

  
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(MQ_AO);
    delay(100);
  }
  baseline = sum / 10;

  Serial.print("Baseline initialized at: ");
  Serial.println(baseline);
  Serial.println("-----------------");
}

void loop() {
  int analogValue = analogRead(MQ_AO);
  int digitalValue = digitalRead(MQ_DO);
  
  Serial.print("MQ2 Analog: ");
  Serial.println(analogValue);

  Serial.print("MQ2 Digital: ");
  Serial.println(digitalValue);

  
  int delta = analogValue - baseline;
  if (delta < 0) delta = 0;

  Serial.print("Delta ADC: ");
  Serial.println(delta);

  if (delta < THRESHOLD_WARN) {
    // NORMAL: Tăng ít hơn 150 đơn vị ADC -> LED xanh sáng
    turnOffAll();
    digitalWrite(LED_GREEN, HIGH);
    Serial.println("Status: NORMAL");
  } 
  else if (delta < THRESHOLD_ALERT) {
    // WARNING: Tăng từ 150 đến dưới 400 đơn vị ADC -> LED vàng sáng
    turnOffAll();
    digitalWrite(LED_YELLOW, HIGH);
    Serial.println("Status: WARNING");
  } 
  else {
    // ALERT: Tăng từ 400 đơn vị ADC trở lên -> LED đỏ sáng và buzzer cảnh báo
    turnOffAll();
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("Status: GAS ALERT");
  }

  Serial.println("-----------------");

  delay(1000);
}