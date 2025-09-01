#define LEDR  0
#define LEDG  2

void setup() 
{ pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
}
void loop() 
{ digitalWrite(LEDR,HIGH); 
  digitalWrite(LEDG,LOW); 
  delay(500);
  digitalWrite(LEDR,LOW); 
  digitalWrite(LEDG,HIGH); 
  delay(500);
}
