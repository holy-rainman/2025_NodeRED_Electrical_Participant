#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#define LEDR 2
#define LEDG 15

// ==== WiFi credentials ====
const char* ssid        = "YOUR_SSID";
const char* password    = "YOUR_PW";

// ==== HiveMQ Cloud credentials ====
const char* mqtt_server = "YOUR_MQTT_SERVER";
const int   mqtt_port   = 8883;
const char* mqtt_user   = "YOUR_MQTT_USERNAME";
const char* mqtt_pass   = "YOUR_MQTT_PW";
WiFiClientSecure espClient;
PubSubClient client(espClient);

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
}

void reconnect() 
{ while (!client.connected()) 
  { Serial.print("Attempting MQTT connection...");
    if (client.connect("ESPClient", mqtt_user, mqtt_pass)) 
    { Serial.println("connected");
      client.subscribe("aim/LedControl");
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
}

void loop() 
{ if (!client.connected()) 
    reconnect();
  client.loop();
}
