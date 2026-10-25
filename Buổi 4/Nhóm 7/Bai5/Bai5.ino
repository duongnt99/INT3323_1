const int IN1 = 25;
const int IN2 = 26;
const int BUTTON_PIN = 23;
const int BUZZER = 18;

const int LED_DO = 19;
const int LED_VANG = 21;
const int LED_XANH = 22;

enum GameState {
  STATE_IDLE,       // Chờ người chơi nhấn START
  STATE_RUNNING,    // Motor chạy, chờ ngẫu nhiên 3-6s
  STATE_WARNING,    // Cảnh báo sự cố (nhấp nháy LED vàng, còi kêu, giới hạn 2s)
  STATE_SUCCESS,    // Xử lý thành công (LED xanh nháy 2s)
  STATE_FAIL        // Thất bại / Phạm luật (LED đỏ nháy + Còi kêu 2s)
};

GameState currentState = STATE_IDLE;

// Biến quản lý thời gian
unsigned long stateStartTime = 0;
unsigned long randomDelayTime = 0;
unsigned long previousBlinkMillis = 0;
bool toggleState = false;

bool isButtonPressed() {
  static int lastReading = HIGH;
  static int stableState = HIGH;
  static unsigned long lastDebounceTime = 0;
  bool pressed = false;

  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > 50) {
    if (reading != stableState) {
      stableState = reading;
      if (stableState == LOW) {
        pressed = true;
      }
    }
  }
  lastReading = reading;
  return pressed;
}

void stopAllOutputs() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(LED_DO, LOW);
  digitalWrite(LED_VANG, LOW);
  digitalWrite(LED_XANH, LOW);
  digitalWrite(BUZZER, HIGH);
}

void setIdleState() {
  stopAllOutputs();
  digitalWrite(LED_DO, HIGH);
  currentState = STATE_IDLE;
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_DO, OUTPUT);
  pinMode(LED_VANG, OUTPUT);
  pinMode(LED_XANH, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  randomSeed(analogRead(34));

  setIdleState();
}

void loop() {
  unsigned long currentMillis = millis();
  bool buttonPressed = isButtonPressed();

  switch (currentState) {
    case STATE_IDLE:
      if (buttonPressed) {
        stopAllOutputs();
        digitalWrite(IN1, HIGH);
        digitalWrite(LED_XANH, HIGH);
        
        // Tạo khoảng thời gian ngẫu nhiên từ 3000ms - 6000ms
        randomDelayTime = random(3000, 6001); 
        stateStartTime = currentMillis;
        currentState = STATE_RUNNING;
      }
      break;

    case STATE_RUNNING:
      // Nếu nhấn nút trước khi cảnh báo -> Phạm luật -> Thất bại
      if (buttonPressed) {
        stopAllOutputs();
        stateStartTime = currentMillis;
        previousBlinkMillis = currentMillis;
        currentState = STATE_FAIL;
      } 
      // Hết thời gian ngẫu nhiên -> Chuyển sang trạng thái cảnh báo
      else if (currentMillis - stateStartTime >= randomDelayTime) {
        digitalWrite(LED_XANH, LOW);
        stateStartTime = currentMillis;
        previousBlinkMillis = currentMillis;
        toggleState = false;
        currentState = STATE_WARNING;
      }
      break;

    case STATE_WARNING:
      // Nhấn kịp trong vòng 2 giây -> Thành công
      if (buttonPressed) {
        stopAllOutputs();
        stateStartTime = currentMillis;
        previousBlinkMillis = currentMillis;
        toggleState = false;
        currentState = STATE_SUCCESS;
      } 
      // Quá 2 giây không nhấn -> Thất bại
      else if (currentMillis - stateStartTime >= 2000) {
        stopAllOutputs();
        stateStartTime = currentMillis;
        previousBlinkMillis = currentMillis;
        toggleState = false;
        currentState = STATE_FAIL;
      } 
      // LED vàng nhấp nháy và còi phát tín hiệu (nhịp 150ms)
      else {
        if (currentMillis - previousBlinkMillis >= 150) {
          previousBlinkMillis = currentMillis;
          toggleState = !toggleState;
          digitalWrite(LED_VANG, toggleState ? HIGH : LOW);
          digitalWrite(BUZZER, toggleState ? LOW : HIGH);
        }
      }
      break;

    case STATE_SUCCESS:
      if (currentMillis - stateStartTime < 2000) {
        if (currentMillis - previousBlinkMillis >= 200) {
          previousBlinkMillis = currentMillis;
          toggleState = !toggleState;
          digitalWrite(LED_XANH, toggleState ? HIGH : LOW);
        }
      } else {
        setIdleState();
      }
      break;

    case STATE_FAIL:
      if (currentMillis - stateStartTime < 2000) {
        if (currentMillis - previousBlinkMillis >= 200) {
          previousBlinkMillis = currentMillis;
          toggleState = !toggleState;
          digitalWrite(LED_DO, toggleState ? HIGH : LOW);
          digitalWrite(BUZZER, toggleState ? LOW : HIGH);
        }
      } else {
        setIdleState();
      }
      break;
  }
}