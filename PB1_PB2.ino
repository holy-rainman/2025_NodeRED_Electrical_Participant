#define LEDR  0
#define LEDG  2
#define RLY1  23
#define RLY2  22
#define PB1   18
#define PB2   19

void setup() 
{ pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  pinMode(RLY1,OUTPUT);
  pinMode(RLY2,OUTPUT);
  digitalWrite(RLY1,HIGH);
  digitalWrite(RLY2,HIGH);
  
  pinMode(PB1,INPUT_PULLUP);
  pinMode(PB2,INPUT_PULLDOWN);
}
void loop() 
{ if(digitalRead(PB1)==0) { digitalWrite(RLY1,LOW);  digitalWrite(RLY2,HIGH); }
  if(digitalRead(PB2)==1) { digitalWrite(RLY1,HIGH); digitalWrite(RLY2,LOW);  }
  if(digitalRead(PB1)==1 && digitalRead(PB2)==0)
  { digitalWrite(RLY1,HIGH);
    digitalWrite(RLY2,HIGH);
  }
}
