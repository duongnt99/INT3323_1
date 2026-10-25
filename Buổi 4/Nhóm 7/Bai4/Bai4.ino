const int IN1 = 25;
const int IN2 = 26;
const int BUTTON_PIN = 23;
const int BUZZER = 18;

const int LED_DO = 19;
const int LED_VANG = 21;
const int LED_XANH = 22;

void turnOffAllOutputs() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(LED_DO, LOW);
  digitalWrite(LED_VANG, LOW);
  digitalWrite(LED_XANH, LOW);
  digitalWrite(BUZZER, HIGH);
}

void startMotor() {
  turnOffAllOutputs();
  digitalWrite(IN1, HIGH);
  digitalWrite(LED_XANH, HIGH);
}

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(LED_DO, OUTPUT);
  pinMode(LED_VANG, OUTPUT);
  pinMode(LED_XANH, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  startMotor();
}

int state;
// 0: RUNNING
// 1: EMERGENCY_STOP
// 2: RESTARTING

long long previousMillisBlink = 0;
long long restartStartMillis = 0;
long long lastDebounceTime = 0;
const long long debounceDelay = 50;

int lastButtonState = HIGH;
int buttonState = HIGH;
bool toggleState = false;

void loop() {
  long long currentMillis = millis();

  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
  }

  bool buttonPressed = false;
  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        buttonPressed = true;
      }
    }
  }
  lastButtonState = reading;


  switch (state) {
    case 0:
      // Nhấn nút -> Dừng motor, chuyển sang trạng thái dừng khẩn cấp (1)
      if (buttonPressed) {
        state = 1;
        turnOffAllOutputs();
        previousMillisBlink = currentMillis;
        toggleState = false;
      }
      break;

    case 1:
      // Nhấn nút -> Chuyển sang trạng thái chuẩn bị khởi động lại (2)
      if (buttonPressed) {
        state = 2;
        restartStartMillis = currentMillis;
        turnOffAllOutputs();
        previousMillisBlink = currentMillis;
        toggleState = false;
      } else {
        if (currentMillis - previousMillisBlink >= 500) {
          previousMillisBlink = currentMillis;
          toggleState = !toggleState;
          digitalWrite(LED_DO, toggleState ? HIGH : LOW);
          digitalWrite(BUZZER, toggleState ? LOW : HIGH);
        }
      }
      break;

    case 2:
      // Trong lúc chuẩn bị khởi động, nếu nhấn nút -> Hủy và về trạng thái dừng (1)
      if (buttonPressed) {
        state = 1;
        turnOffAllOutputs();
        previousMillisBlink = currentMillis;
        toggleState = false;
      } 
      // Hết 3 giây chuẩn bị -> Cho motor hoạt động trở lại (0)
      else if (currentMillis - restartStartMillis >= 3000) {
        state = 0;
        startMotor();
      } 
      // LED vàng nhấp nháy trong 3 giây (chu kỳ 250ms)
      else {
        if (currentMillis - previousMillisBlink >= 250) {
          previousMillisBlink = currentMillis;
          toggleState = !toggleState;
          digitalWrite(LED_VANG, toggleState ? HIGH : LOW);
        }
      }
      break;
  }
}
