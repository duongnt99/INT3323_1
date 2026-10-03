#define R 22
#define Y 23
#define G 21
#define B 19

void setup(){
  pinMode(R,OUTPUT);
  pinMode(Y,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
}

void loop(){
  digitalWrite(R,1);
  delay(2000);
  digitalWrite(R,0);

  for(int i=5;i>0;i--){
    if(i>3){
      digitalWrite(Y,1);
      tone(B,1000);
      delay(300);
      noTone(B);
      digitalWrite(Y,0);
      delay(700);
    }
    else if(i==3){
      digitalWrite(Y,1);
      tone(B,1000);
      delay(200);
      noTone(B);
      digitalWrite(Y,0);
      delay(300);

      digitalWrite(Y,1);
      tone(B,1000);
      delay(200);
      noTone(B);
      digitalWrite(Y,0);
      delay(300);
    }
    else if(i==2){
      digitalWrite(Y,1);
      tone(B,1000);
      delay(150);
      noTone(B);
      digitalWrite(Y,0);
      delay(200);

      digitalWrite(Y,1);
      tone(B,1000);
      delay(150);
      noTone(B);
      digitalWrite(Y,0);
      delay(200);

      digitalWrite(Y,1);
      tone(B,1000);
      delay(150);
      noTone(B);
      digitalWrite(Y,0);
      delay(150);
    }
    else{
      digitalWrite(Y,1);
      tone(B,1000);
      delay(100);
      noTone(B);
      digitalWrite(Y,0);
      delay(150);

      digitalWrite(Y,1);
      tone(B,1000);
      delay(100);
      noTone(B);
      digitalWrite(Y,0);
      delay(150);

      digitalWrite(Y,1);
      tone(B,1000);
      delay(100);
      noTone(B);
      digitalWrite(Y,0);
      delay(150);

      digitalWrite(Y,1);
      tone(B,1000);
      delay(100);
      noTone(B);
      digitalWrite(Y,0);
    }
  }

  digitalWrite(G,1);
  tone(B,1000);
  delay(2000);
  noTone(B);

  for(int i=0;i<5;i++){
    digitalWrite(R,1);
    digitalWrite(Y,1);
    digitalWrite(G,1);
    delay(300);

    digitalWrite(R,0);
    digitalWrite(Y,0);
    digitalWrite(G,0);
    delay(300);
  }

  digitalWrite(G,0);
  delay(5000);
}