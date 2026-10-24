const int SOIL_ANALOG = 35;
const int SOIL_DIGITAL = 25;
const int RED_LED = 16;
const int YELLOW_LED = 17;
const int GREEN_LED = 18;
const int BUZZER = 13;

const int DRY_THRESHOLD = 4095;
const int WET_THRESHOLD = 2000;

void setup()
{
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SOIL_DIGITAL, INPUT);
  analogReadResolution(12);
}

void loop()
{
  delay(1000);

  int analog = analogRead(SOIL_ANALOG);
  int digital = digitalRead(SOIL_DIGITAL);

  int dryness = map(analog, WET_THRESHOLD, DRY_THRESHOLD, 0, 100);
  dryness = constrain(dryness, 0, 100);

  Serial.printf("Analog moisture: %d | Digital state: %d | Wetness: %d%%\n", analog, digital, 100 - dryness);
  if(dryness < 35)
  {
    Serial.println("WET");
    setLed(GREEN_LED);
    setBuzzer(false);
  }
  else if(dryness < 70)
  {
    Serial.println("MODERATE");
    setLed(YELLOW_LED);
    setBuzzer(false);
  }
  else
  {
    Serial.println("DRY");
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