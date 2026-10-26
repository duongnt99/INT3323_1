// Bai 5: Tro choi phan xa may moc
const int PIN_BUTTON    = 18;
const int PIN_LED_RED   = 19;
const int PIN_LED_YEL   = 22;
const int PIN_LED_GREEN = 21;
const int PIN_BUZZER    = 4;
const int PIN_ENA       = 25;
const int PIN_IN1       = 26;
const int PIN_IN2       = 27;

enum GameState {
  GAME_IDLE,    // Cho nhan START (LED do sang)
  GAME_RUNNING, // Motor chay, LED xanh sang, doi thoi gian ngau nhien
  GAME_ALERT    // LED vang nhap nhay + Buzzer keu (cho phan xa trong 2s)
};

GameState gameState = GAME_IDLE;
unsigned long alertStartTime = 0;
unsigned long randomDelayMs = 0;
unsigned long runningStartTime = 0;

void resetGame() {
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_YEL, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_BUZZER, LOW);
  gameState = GAME_IDLE;
}

void reportSuccess() {
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_LED_YEL, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  // LED xanh nhap nhay trong 2 giay
  for (int i = 0; i < 5; i++) {
    digitalWrite(PIN_LED_GREEN, HIGH);
    delay(200);
    digitalWrite(PIN_LED_GREEN, LOW);
    delay(200);
  }
  resetGame();
}

void reportFailure() {
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_YEL, LOW);

  // LED do nhay kem buzzer canh bao that bai trong 2 giay
  for (int i = 0; i < 4; i++) {
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_BUZZER, HIGH);
    delay(250);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
    delay(250);
  }
  resetGame();
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YEL, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);

  // Khoi tao hat giong ngau nhien tu tin hieu nhieu tren chan noi tiep ADC trong
  randomSeed(analogRead(34));

  resetGame();
}

void loop() {
  unsigned long now = millis();

  // Doc nut nhan (tich cuc muc LOW)
  bool buttonPressed = false;
  if (digitalRead(PIN_BUTTON) == LOW) {
    delay(30); // Loc nhieu phím
    if (digitalRead(PIN_BUTTON) == LOW) {
      buttonPressed = true;
      while (digitalRead(PIN_BUTTON) == LOW); // Cho nha nut
    }
  }

  switch (gameState) {
    case GAME_IDLE:
      if (buttonPressed) {
        // Bat dau tro choi
        digitalWrite(PIN_LED_RED, LOW);
        digitalWrite(PIN_LED_GREEN, HIGH);
        analogWrite(PIN_ENA, 255);

        randomDelayMs = random(3000, 6001); // Thoi gian ngau nhien tu 3 den 6 giay
        runningStartTime = millis();
        gameState = GAME_RUNNING;
      }
      break;

    case GAME_RUNNING:
      // Neu nguoi choi nhan nut truoc khi co canh bao -> Pham quy
      if (buttonPressed) {
        reportFailure();
        break;
      }

      // Het thoi gian cho ngau nhien -> Chuyen sang trang thai canh bao
      if (now - runningStartTime >= randomDelayMs) {
        digitalWrite(PIN_LED_GREEN, LOW);
        alertStartTime = millis();
        gameState = GAME_ALERT;
      }
      break;

    case GAME_ALERT:
      // Trong luc canh bao: LED vang nhap nhay va buzzer keu
      digitalWrite(PIN_LED_YEL, (now / 100) % 2 == 0);
      digitalWrite(PIN_BUZZER, HIGH);

      // Nhan nut kip trong vong 2 giay (2000 ms)
      if (buttonPressed) {
        reportSuccess();
        break;
      }

      // Qua 2 giay khong nhan nut -> That bai
      if (now - alertStartTime > 2000) {
        reportFailure();
      }
      break;
  }
}