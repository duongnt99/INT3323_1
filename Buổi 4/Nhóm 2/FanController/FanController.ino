const int LED_RED_PIN = 23;  
const int LED_GREEN_PIN = 22;
const int MOTOR_PIN = 25;
const int MOTOR_AUX_PIN = 26;
const int BUTTON_PIN = 13;

int ledState = LOW;          
int lastButtonState = HIGH;
int buttonState;           

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;  

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_AUX_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 

  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(MOTOR_AUX_PIN, LOW);
}

void loop() {
  // Đọc trạng thái hiện tại của chân nút nhấn
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        ledState = !ledState;
        updateStatus();
      }
    }
  }

  // Lưu lại trạng thái để so sánh cho vòng lặp tiếp theo
  lastButtonState = reading;
}

void updateStatus()
{
  switch(ledState)
  {
    case true:
      digitalWrite(LED_GREEN_PIN, HIGH);
      digitalWrite(LED_RED_PIN, LOW);
      digitalWrite(MOTOR_PIN, HIGH);
      Serial.println("Motor ON!");
      break;

    case false:
      digitalWrite(LED_RED_PIN, HIGH);
      digitalWrite(LED_GREEN_PIN, LOW);
      digitalWrite(MOTOR_PIN, LOW);
      Serial.println("Motor OFF!");
  }
}