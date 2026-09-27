const int LED[3] = {23, 22, 21};
int n = 3;

void setup() {
  for(int i=0; i<n; i++)
    pinMode(LED[i], OUTPUT);
}

void loop() {
  for(int i=0; i<n; i++)
  {
    digitalWrite(LED[i], HIGH);
    for(int j=0; j<n; j++)
      if(i != j) digitalWrite(LED[j], LOW);
    delay(1000);
  }

  for(int i=n-2; i>0; i--)
  {
    digitalWrite(LED[i], HIGH);
    for(int j=0; j<n; j++)
      if(i != j) digitalWrite(LED[j], LOW);
    delay(1000);
  }
}