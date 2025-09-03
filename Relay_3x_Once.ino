#define LEDR  2
#define LEDG  15
#define RLY1  23
#define RLY2  22

void setup() 
{ pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  pinMode(RLY1,OUTPUT);
  pinMode(RLY2,OUTPUT);
  digitalWrite(RLY1,HIGH);
  digitalWrite(RLY2,HIGH);
}
void loop() 
{ for(int i=0;i<3;i++)
  { digitalWrite(RLY1,LOW);   delay(500);
    digitalWrite(RLY1,HIGH);  delay(500);
    digitalWrite(RLY2,LOW);   delay(500);
    digitalWrite(RLY2,HIGH);  delay(500);
  }
  while(1)
  { digitalWrite(LEDR,HIGH);  delay(200);
    digitalWrite(LEDR,LOW);   delay(200);
  }
}

