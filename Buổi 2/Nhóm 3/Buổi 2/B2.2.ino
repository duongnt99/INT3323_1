const int LED1 = 33; 
const int LED2 = 34; 
const int LED3 = 35; 

const int chuoi[6] = {LED1, LED2, LED3, LED1, LED2, LED3};

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

void loop() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(chuoi[i], HIGH); 
    delay(1000);                   
    digitalWrite(chuoi[i], LOW);  
  }
}