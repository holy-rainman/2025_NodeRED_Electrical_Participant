#define LEDR  15
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
{ if(digitalRead(PB2)==1) 
  { digitalWrite(RLY2,LOW);  delay(500);
    
    while(digitalRead(PB2)==1)
    { digitalWrite(RLY2,HIGH);
      digitalWrite(LEDG,HIGH);
    }
    
    digitalWrite(LEDG,LOW);
    digitalWrite(LEDR,HIGH);
    delay(1000);
  }
  if(digitalRead(PB1)==1 && digitalRead(PB2)==0)
  { digitalWrite(RLY1,HIGH);  digitalWrite(LEDR,LOW);
    digitalWrite(RLY2,HIGH);  digitalWrite(LEDG,LOW);  
  }
}
