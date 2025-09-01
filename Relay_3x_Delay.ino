#define LEDR  15
#define LEDG  2
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
  delay(3000);
}

