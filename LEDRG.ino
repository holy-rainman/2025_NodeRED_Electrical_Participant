#define LEDR  2
#define LEDG  15

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
