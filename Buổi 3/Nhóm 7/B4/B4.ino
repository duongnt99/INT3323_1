const int LED_XANH = 21;
const int LED_VANG = 22;
const int LED_DO = 23;
const int BUZZER = 19;

void setup() {
  pinMode(LED_XANH, OUTPUT);
  pinMode(LED_VANG, OUTPUT);
  pinMode(LED_DO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}
void tat_tat_ca_den() {
  digitalWrite(LED_XANH, LOW);
  digitalWrite(LED_VANG, LOW);
  digitalWrite(LED_DO, LOW);
}

void loop() {
  digitalWrite(BUZZER, HIGH);
  tat_tat_ca_den();
  digitalWrite(LED_XANH, HIGH);
  delay(4000);

  for(int i=0; i<2; i++)
  {
    digitalWrite(BUZZER, LOW);
    delay(250);
    digitalWrite(BUZZER, HIGH);
    delay(3000-250);
  }

  tat_tat_ca_den();
  digitalWrite(LED_VANG, HIGH);
  delay(5000);

  tat_tat_ca_den(); 
  digitalWrite(LED_DO, HIGH);
  delay(3000);
  
  for(int i=0; i<3; i++)
  {
    digitalWrite(BUZZER, LOW);
    delay(250);
    digitalWrite(BUZZER, HIGH);
    delay(1000-250);
  }
}