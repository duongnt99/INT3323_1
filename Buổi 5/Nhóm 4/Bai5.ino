#include "DHT.h"

const int DHTPIN = 4;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

const int soilAoPin = 34;
const int mq2AoPin = 32;
const int mq2DoPin = 35;
const int ledGreen = 14;
const int ledYellow = 27;
const int ledRed = 26;
const int buzzerPin = 33;

int wetValue = 1000;
int dryValue = 4000;
int mq2Baseline = 0;

unsigned long lastSampleTime = 0;
unsigned long lastYellowBlink = 0;
unsigned long lastRedBlink = 0;
unsigned long lastBuzzerToggle = 0;

bool yellowState = false;
bool redState = false;
bool buzzerState = false;

int systemState = 0; 

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(soilAoPin, INPUT);
  pinMode(mq2AoPin, INPUT);
  pinMode(mq2DoPin, INPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(buzzerPin, LOW);

  for (int i = 0; i < 10; i++) {
    delay(1000);
  }

  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(mq2AoPin);
    delay(100);
  }
  mq2Baseline = sum / 10;
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastSampleTime >= 2000) {
    lastSampleTime = currentMillis;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int soilAdc = analogRead(soilAoPin);
    int mq2Ao = analogRead(mq2AoPin);
    int mq2Do = digitalRead(mq2DoPin);

    int soilDryness = map(soilAdc, wetValue, dryValue, 0, 100);
    soilDryness = constrain(soilDryness, 0, 100);

    bool dhtError = (isnan(h) || isnan(t));

    Serial.print("Temp: ");
    if (dhtError) Serial.print("ERROR"); else Serial.print(t);
    Serial.print(" C | Hum: ");
    if (dhtError) Serial.print("ERROR"); else Serial.print(h);
    Serial.print(" % | Soil: ");
    Serial.print(soilDryness);
    Serial.print(" % | MQ2 AO: ");
    Serial.print(mq2Ao);
    Serial.print(" | MQ2 DO: ");
    Serial.println(mq2Do);

    if (dhtError) {
      Serial.println("SENSOR ERROR");
    }

    if ((mq2Ao - mq2Baseline) >= 400) {
      systemState = 3;
    } 
    else if (!dhtError && t >= 30.0 && soilDryness >= 70) {
      systemState = 2;
    } 
    else if (dhtError || soilDryness >= 55 || (!dhtError && (t >= 30.0 || h < 40.0))) {
      systemState = 1;
    } 
    else {
      systemState = 0;
    }
  }

  if (systemState == 0) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);
  } 
  else if (systemState == 1) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);
    
    if (currentMillis - lastYellowBlink >= 500) {
      lastYellowBlink = currentMillis;
      yellowState = !yellowState;
      digitalWrite(ledYellow, yellowState);
    }
  } 
  else if (systemState == 2) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
    
    if (!buzzerState && (currentMillis - lastBuzzerToggle >= 2000)) {
      lastBuzzerToggle = currentMillis;
      buzzerState = true;
      digitalWrite(buzzerPin, HIGH);
    } 
    else if (buzzerState && (currentMillis - lastBuzzerToggle >= 200)) {
      lastBuzzerToggle = currentMillis;
      buzzerState = false;
      digitalWrite(buzzerPin, LOW);
    }
  } 
  else if (systemState == 3) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    
    if (currentMillis - lastRedBlink >= 150) {
      lastRedBlink = currentMillis;
      redState = !redState;
      digitalWrite(ledRed, redState);
      digitalWrite(buzzerPin, redState);
    }
  }
}