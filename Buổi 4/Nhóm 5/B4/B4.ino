// Bai 3: Bang chuyen dong goi tu dong
const int PIN_START     = 18;
const int PIN_LED_RED   = 19;
const int PIN_LED_YEL   = 22;
const int PIN_LED_GREEN = 21;
const int PIN_BUZZER    = 4;
const int PIN_ENA       = 25;
const int PIN_IN1       = 26;
const int PIN_IN2       = 27;

// Thoi gian bang chuyen van hanh (tuy chon 3000, 5000 hoac 10000 ms)
const unsigned long CONVEYOR_RUNTIME = 5000; 

bool isBusy = false;

void setupSystemReady() {
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_YEL, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_BUZZER, LOW);
  isBusy = false;
}

void setup() {
  pinMode(PIN_START, INPUT_PULLUP);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YEL, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);

  setupSystemReady();
}

void loop() {
  // Chi tiep nhan nut nhan neu he thong dang ranh (khong ban chu trinh)
  if (!isBusy && digitalRead(PIN_START) == LOW) {
    delay(50); // Loc nhieu co hoc
    if (digitalRead(PIN_START) == LOW) {
      isBusy = true;

      // 1. LED do tat, LED vang sang va dem nguoc 3 tieng (moi tieng cach nhau 1s)
      digitalWrite(PIN_LED_RED, LOW);
      digitalWrite(PIN_LED_YEL, HIGH);
      for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_BUZZER, HIGH);
        delay(150);
        digitalWrite(PIN_BUZZER, LOW);
        delay(850);
      }
      digitalWrite(PIN_LED_YEL, LOW);

      // 2. LED xanh sang, chay dong co trong 5 giay
      digitalWrite(PIN_LED_GREEN, HIGH);
      analogWrite(PIN_ENA, 255);
      delay(CONVEYOR_RUNTIME);

      // 3. Dung dong co, LED xanh nhap nhay 3 lan bao hoan thanh
      analogWrite(PIN_ENA, 0);
      for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_LED_GREEN, LOW);
        delay(250);
        digitalWrite(PIN_LED_GREEN, HIGH);
        delay(250);
      }

      // 4. Quay lai trang thai ban dau
      setupSystemReady();
    }
  }
}