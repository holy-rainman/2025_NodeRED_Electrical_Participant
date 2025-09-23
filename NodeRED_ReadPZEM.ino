#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <PZEM004Tv30.h>

// Define RX/TX pins for ESP32 
#define PZEM_RX 16   // GPIO16
#define PZEM_TX 17   // GPIO17
PZEM004Tv30 pzem(Serial2, PZEM_RX, PZEM_TX);

// Define GPIO pins for ESP32 
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

  // Initialize Serial2 (baud is fixed at 9600 for PZEM-004T)
  Serial2.begin(9600, SERIAL_8N1, PZEM_RX, PZEM_TX);
  Serial.println("PZEM-004T ESP32 Test");
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
  //Serial.println(ldrValue);

  readPZEM();
}

char tx2buf[200];
char buf[16];   // temp buffer for float-to-string conversion

void readPZEM() 
{ float voltage   = pzem.voltage();
  float current   = pzem.current();
  float power     = pzem.power();
  float energy    = pzem.energy();
  float frequency = pzem.frequency();
  float pf        = pzem.pf();

  if (!isnan(voltage) && !isnan(current) && !isnan(power) &&
      !isnan(energy) && !isnan(frequency) && !isnan(pf)) 
  { // Print to Serial
    Serial.print("Voltage:");       Serial.print(voltage);      Serial.println(" V");
    Serial.print("Current:");       Serial.print(current);      Serial.println(" A");
    Serial.print("Power:");         Serial.print(power);        Serial.println(" W");
    Serial.print("Energy:");        Serial.print(energy);       Serial.println(" kWh");
    Serial.print("Frequency:");     Serial.print(frequency);    Serial.println(" Hz");
    Serial.print("Power Factor:");  Serial.println(pf);         Serial.println("------\n");  
    
    // Publish each value separately
    dtostrf(voltage,      6, 2, buf); client.publish("aim/voltage",   buf);
    dtostrf(current*1000, 6, 2, buf); client.publish("aim/current",   buf);
    dtostrf(power,        6, 2, buf); client.publish("aim/power",     buf);
    dtostrf(energy,       6, 2, buf); client.publish("aim/energy",    buf);
    dtostrf(frequency,    6, 2, buf); client.publish("aim/frequency", buf);
    dtostrf(pf,           6, 2, buf); client.publish("aim/pf",        buf);
  }

  else 
  { if (isnan(voltage))   Serial.println("Error reading voltage");
    if (isnan(current))   Serial.println("Error reading current");
    if (isnan(power))     Serial.println("Error reading power");
    if (isnan(energy))    Serial.println("Error reading energy");
    if (isnan(frequency)) Serial.println("Error reading frequency");
    if (isnan(pf))        Serial.println("Error reading power factor");
  }
}
