const int SOIL_AO = 35;
const int SOIL_DO = 19;

const int LED_GREEN = 23;
const int LED_YELLOW = 22;
const int LED_RED = 25;
const int BUZZER_PIN = 32;

const int WET_VALUE = 1300;
const int DRY_VALUE = 3200;

void turOffAll(){
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, HIGH);
}

void setup() {
  Serial.begin(9600);

  pinMode(SOIL_DO, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  analogReadResolution(12);
}

void loop() {
  int raw = analogRead(SOIL_AO);
  int digitalState = digitalRead(SOIL_DO);

  int dryness = map(
    raw,
    WET_VALUE,
    DRY_VALUE,
    0,
    100
  );

  dryness = constrain(dryness, 0, 100);

  Serial.print("ADC: ");
  Serial.println(raw);

  Serial.print("Dryness: ");
  Serial.print(dryness);
  Serial.println("%");

  Serial.print("Digital: ");
  Serial.println(digitalState);

  if (dryness < 35) {
    // WET: Độ khô < 35% -> LED xanh sáng
    turOffAll();
    digitalWrite(LED_GREEN, HIGH);
    Serial.println("Status: WET");
  } 
  else if (dryness < 70) {
    // MODERATE: Độ khô từ 35% đến dưới 70% -> LED vàng sáng
    turOffAll();
    digitalWrite(LED_YELLOW, HIGH);
    Serial.println("Status: MODERATE");
  } 
  else {
    // DRY: Độ khô từ 70% trở lên -> LED đỏ sáng và buzzer cảnh báo
    turOffAll();
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("Status: DRY");
  }

  Serial.println("--------------------");

  delay(1000);
}