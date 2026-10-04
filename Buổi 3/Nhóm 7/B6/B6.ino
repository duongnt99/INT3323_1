const int BUTTON_PIN = 19;
const int LED_RED    = 21;
const int LED_YELLOW = 22;
const int LED_GREEN  = 23;
int currentLed = 21;

void deactivateAllLed() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  deactivateAllLed();	
  digitalWrite(LED_RED,HIGH);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  if(reading == LOW)
  {
    currentLed = (currentLed + 1)%21 + 21;
    deactivateAllLed();
	  digitalWrite(currentLed, HIGH);
    delay(50);
    while(digitalRead(BUTTON_PIN) == LOW) delay(50);
  }
}