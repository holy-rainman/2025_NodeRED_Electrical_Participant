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
#define LDR  34

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

int prevPB1 = HIGH;  // anggap butang idle = HIGH
int prevPB2 = LOW;   // anggap butang idle = LOW
String prevLdrStatus = "";   // simpan status LDR sebelum ini

void loop() 
{ if (!client.connected()) 
    reconnect();
  client.loop();
  
  int currPB1 = digitalRead(PB1);
  int currPB2 = digitalRead(PB2);

  if(currPB1 == LOW && prevPB1 == HIGH)   // PB1 pressed
  { client.publish("aim/Button1Status", "Button 1 pressed");
    delay(200); // debounce
  }
  if(currPB1 == HIGH && prevPB1 == LOW)  // PB1 released (idle)
  { client.publish("aim/Button1Status", "Button 1 idle");
    delay(200);
  }
  if(currPB2 == HIGH && prevPB2 == LOW)  // PB2 pressed 
  { client.publish("aim/Button2Status", "Button 2 pressed");
    delay(200);
  }
  if(currPB2 == LOW && prevPB2 == HIGH)  // PB2 released (idle)
  { client.publish("aim/Button2Status", "Button 2 idle");
    delay(200);
  }

  prevPB1 = currPB1;  // simpan keadaan terkini untuk loop seterusnya
  prevPB2 = currPB2;

  // === LDR ===
  int ldrValue = analogRead(LDR);
  String currLdrStatus;

  if (ldrValue >= 2800)       currLdrStatus = "Dark";
  else if (ldrValue >= 800)  currLdrStatus = "Room Light";
  else                        currLdrStatus = "Bright Light";

  // hanya publish bila ada perubahan status
  if (currLdrStatus != prevLdrStatus) 
  { client.publish("aim/ldrStatus", currLdrStatus.c_str());
    prevLdrStatus = currLdrStatus;
  }

  Serial.println(ldrValue);
}
