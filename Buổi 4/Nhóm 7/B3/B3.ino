const int IN1 = 25;
const int IN2 = 26;
const int BUTTON_PIN = 23;
const int LED[4] = {0, 19, 21, 22};
const int BUZZER = 18;
const int speed[4] = {0, 100, 180, 255};

void turnOffLight(){
  for(int i=1; i<=3; i++)
    digitalWrite(LED[i], LOW);
}

void ON(int level){
  analogWrite(IN1, speed[level]);
  turnOffLight();
  if (level > 0) digitalWrite(LED[level], HIGH);

  for(int i=1; i<=level; i++)
  {
    digitalWrite(BUZZER, LOW);
    delay(500);
    digitalWrite(BUZZER, HIGH);
    delay(500);
  }
  
  if(level == 0)
  {
    digitalWrite(BUZZER, LOW);
    delay(1000);
    digitalWrite(BUZZER, HIGH);
  }
}

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  for(int i=1; i<=3; i++)
    pinMode(LED[i], OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  digitalWrite(BUZZER, HIGH);
  turnOffLight();
}

int level = 0;
void loop() {
  int reading = digitalRead(BUTTON_PIN);
  if(reading == LOW)
  {
    level = (level + 1)%4;
    ON(level);
    delay(50);
    while(digitalRead(BUTTON_PIN) == LOW) delay(50);
  }
}
