#define B 18
#define R 22
#define Y 23
#define G 21

int i=0;

void setup(){
  pinMode(B,INPUT_PULLUP);
  pinMode(R,OUTPUT);
  pinMode(Y,OUTPUT);
  pinMode(G,OUTPUT);
  digitalWrite(R,1);
}

void loop(){
  if(!digitalRead(B)){
    digitalWrite(R,0);
    digitalWrite(Y,0);
    digitalWrite(G,0);
    i=(i+1)%3;
    if(i==0) digitalWrite(R,1);
    if(i==1) digitalWrite(Y,1);
    if(i==2) digitalWrite(G,1);
    while(!digitalRead(B));
  }
}