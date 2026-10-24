const int MQ2_ANALOG = 34;
const int MQ2_DIIGTAL = 26;
const int RED_LED = 16;
const int YELLOW_LED = 17;
const int GREEN_LED = 18;
const int BUZZER = 13;

const int BASELINE = 350;

void setup()
{
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MQ2_DIIGTAL, INPUT);
  analogReadResolution(12);
}

void loop()
{
  delay(1000);

  int analog = analogRead(MQ2_ANALOG);
  int digital = digitalRead(MQ2_DIIGTAL);

  int difference = abs(analog - BASELINE);

  Serial.printf("Analog gas: %d | Digital state: %d\n", analog, digital);
  if(difference < 150)
  {
    Serial.println("NORMAL");
    setLed(GREEN_LED);
    setBuzzer(false);
  }
  else if(difference < 400)
  {
    Serial.println("WARNING");
    setLed(YELLOW_LED);
    setBuzzer(false);
  }
  else
  {
    Serial.println("ALERT");
    setLed(RED_LED);
    setBuzzer(true);
  }
}

void setLed(int pin)
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  if(pin > 0)
  {
    digitalWrite(pin, HIGH);
  }
}

void setBuzzer(bool on)
{
  if(on)
  {
    tone(BUZZER, 1000);
  }
  else
  {
    noTone(BUZZER);
  }
}