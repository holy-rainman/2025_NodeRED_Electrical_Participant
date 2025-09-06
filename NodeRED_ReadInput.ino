#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#define LEDR 2
#define LEDG 15
#define RLY1 23
#define RLY2 22
#define BUZ  21 
#define PB1  18
#define PB2  19

// ==== WiFi credentials ====
const char* ssid        = "YOUR_SSID";
const char* password    = "YOUR_PW";

// ==== HiveMQ Cloud credentials ====
const char* mqtt_server = "YOUR_URL_SERVER";
const int   mqtt_port   = 8883;
const char* mqtt_user   = "YOUR SERVER_USERNAME";
const char* mqtt_pass   = "YOUR_SERVER_PW";
WiFiClientSecure espClient;
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
    if (client.connect("ESPClient", mqtt_user, mqtt_pass)) 
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

  espClient.setInsecure();
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

void loop() 
{ if (!client.connected()) 
    reconnect();
  client.loop();

  if(digitalRead(PB1)==0)
  { client.publish("aim/Button1Status","Button 1 pressed");
    while(digitalRead(PB1)==0);
    delay(200);
  }
  if(digitalRead(PB2)==1)
  { client.publish("aim/Button2Status","Button 2 pressed");
    while(digitalRead(PB2)==1);
    delay(200);
  }
  if(digitalRead(PB1)==1) client.publish("aim/Button1Status","Button 1 idle");
  if(digitalRead(PB2)==0) client.publish("aim/Button2Status","Button 2 idle");
}
