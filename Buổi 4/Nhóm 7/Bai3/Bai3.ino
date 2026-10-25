const int IN1 = 25;
const int IN2 = 26;
const int BUTTON_PIN = 23;
const int BUZZER = 18;

const int LED_DO = 19;
const int LED_VANG = 21;
const int LED_XANH = 22;

void setReadyState() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  digitalWrite(LED_DO, HIGH);   
  digitalWrite(LED_VANG, LOW);
  digitalWrite(LED_XANH, LOW);
  digitalWrite(BUZZER, HIGH);
}

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(LED_DO, OUTPUT);
  pinMode(LED_VANG, OUTPUT);
  pinMode(LED_XANH, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  setReadyState();
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  if(reading == LOW)
  {
    digitalWrite(LED_DO, LOW);
    digitalWrite(LED_VANG, HIGH);

    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER, LOW);
      delay(200);
      digitalWrite(BUZZER, HIGH);
      delay(800);
    }

    digitalWrite(LED_VANG, LOW);
    digitalWrite(LED_XANH, HIGH);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(5000);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_XANH, HIGH);
      delay(300);
      digitalWrite(LED_XANH, LOW);
      delay(300);
    }
    setReadyState();

    delay(50);
    while (digitalRead(BUTTON_PIN) == LOW) delay(50);
  }
}
