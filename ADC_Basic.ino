#define LEDR  2
#define LEDG  15
#define RLY1  23
#define RLY2  22
#define PB1   18
#define PB2   19
#define LDR   34

void setup() 
{ pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  pinMode(RLY1,OUTPUT);
  pinMode(RLY2,OUTPUT);
  digitalWrite(RLY1,HIGH);
  digitalWrite(RLY2,HIGH);
  
  pinMode(PB1,INPUT_PULLUP);
  pinMode(PB2,INPUT_PULLDOWN);
  Serial.begin(115200);
}
void loop() 
{ int an = analogRead(LDR);
  Serial.println(an);
}
