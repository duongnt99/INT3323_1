const int IN1 = 25;
const int IN2 = 26;
const int BUTTON_PIN = 23;
const int LED_RED = 21;
const int LED_GREEN = 22;
bool state = false;

void ON(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  Serial.println("ON");
}

void OFF()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  Serial.println("OFF");
}

void setup() {
  Serial.begin(9600); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  OFF();
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  if(reading == LOW)
  {
    state = !state;
    if(state) ON();
    else OFF();
    delay(50);
    while(digitalRead(BUTTON_PIN) == LOW) delay(50);
  }
}
