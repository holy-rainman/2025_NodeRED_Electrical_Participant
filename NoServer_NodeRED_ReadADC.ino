#include <WiFi.h>
#include <WiFiClient.h>        // use plain TCP
#include <PubSubClient.h>

#define LEDR 2
#define LEDG 15
#define RLY1 23
#define RLY2 22
#define BUZ  21
#define PB1  18
#define PB2  19
#define LDR  34

// ==== WiFi credentials ====
const char* ssid        = "YOUR SSID";
const char* password    = "YOUR_PW";

// ==== Mosquitto local broker ====
const char* mqtt_server = "LOCAL_SERVER_IP";   // your PC/RPi IP
const int   mqtt_port   = 1883;              // default Mosquitto port
// If allow_anonymous true → leave empty strings
const char* mqtt_user   = ""; 
const char* mqtt_pass   = ""; 

WiFiClient espClient;        // plain WiFi client
PubSubClient client(espClient);

void beep(int bil, int tempoh)
{ for(int i=0;i<bil;i++)
  { digitalWrite(BUZ,HIGH); delay(tempoh);
    digitalWrite(BUZ,LOW);  delay(tempoh);
  }
}

void setup_wifi() 
{ Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  { delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{ String msg = "";
  for (int i = 0; i < length; i++) 
    msg += (char)payload[i];

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (msg == "ledr_1") digitalWrite(LEDR, HIGH);
  if (msg == "ledr_0") digitalWrite(LEDR, LOW);
  if (msg == "ledg_1") digitalWrite(LEDG, HIGH);
  if (msg == "ledg_0") digitalWrite(LEDG, LOW);

  if (msg == "rly1_1") digitalWrite(RLY1, LOW);
  if (msg == "rly1_0") digitalWrite(RLY1, HIGH);
  if (msg == "rly2_1") digitalWrite(RLY2, LOW);
  if (msg == "rly2_0") digitalWrite(RLY2, HIGH);

  beep(1,50);
}

void reconnect() 
{ while (!client.connected()) 
  { Serial.print("Attempting MQTT connection...");
    // anonymous
    if (client.connect("ESPClient")) 
    { Serial.println("connected");
      client.subscribe("aim/LedControl");
      client.subscribe("aim/RlyControl");
    } 
    else 
    { Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() 
{ Serial.begin(115200);
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  pinMode(LEDR,OUTPUT);  digitalWrite(LEDR,LOW);
  pinMode(LEDG,OUTPUT);  digitalWrite(LEDG,LOW);

  pinMode(RLY1,OUTPUT);  digitalWrite(RLY1,HIGH);
  pinMode(RLY2,OUTPUT);  digitalWrite(RLY2,HIGH);
  pinMode(BUZ,OUTPUT);

  pinMode(PB1,INPUT_PULLUP);
  pinMode(PB2,INPUT_PULLDOWN);
}

// Track previous states
static int lastPB1 = HIGH;
static int lastPB2 = LOW;

void loop() 
{ if (!client.connected()) 
    reconnect();
  client.loop();

  int pb1 = digitalRead(PB1);
  int pb2 = digitalRead(PB2);

  // Button 1 logic
  if(pb1 == LOW && lastPB1 == HIGH) client.publish("aim/Button1Status","Button 1 pressed");
  if(pb1 == HIGH && lastPB1 == LOW) client.publish("aim/Button1Status","Button 1 idle");
  lastPB1 = pb1;

  // Button 2 logic
  if(pb2 == HIGH && lastPB2 == LOW) client.publish("aim/Button2Status","Button 2 pressed");
  if(pb2 == LOW && lastPB2 == HIGH) client.publish("aim/Button2Status","Button 2 idle");
  lastPB2 = pb2;

  if(analogRead(LDR)>=2800)      client.publish("aim/ldrStatus","Dark");
  else if(analogRead(LDR)>=1800) client.publish("aim/ldrStatus","Room Light");
  else                           client.publish("aim/ldrStatus","Bright Light");  
  Serial.println(analogRead(LDR));
}
