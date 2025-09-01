#include <PZEM004Tv30.h>
#define PZEM_RX_PIN 16   // Pin ESP32 RX sambung ke TX modul PZEM
#define PZEM_TX_PIN 17   // Pin ESP32 TX sambung ke RX modul PZEM
HardwareSerial PZEMSerial(1);
PZEM004Tv30 pzem(PZEMSerial, PZEM_RX_PIN, PZEM_TX_PIN);

float voltage,current,power,energy,frequency,pf;

void setup() 
{ Serial.begin(115200);

  PZEMSerial.begin(9600, SERIAL_8N1, PZEM_RX_PIN, PZEM_TX_PIN);
  Serial.println("PZEM Monitor Started");
}

void loop() 
{ readPZEM();
}

void readPZEM()
{ Serial.print("Custom Address: ");
  Serial.println(pzem.readAddress(), HEX);

  // Baca data dari sensor
  voltage   = pzem.voltage();
  current   = pzem.current();
  power     = pzem.power();
  energy    = pzem.energy();
  frequency = pzem.frequency();
  pf        = pzem.pf();

  // Semak jika bacaan valid
  if (isnan(voltage))       Serial.println("Error reading voltage");
  else if (isnan(current))  Serial.println("Error reading current");
  else if (isnan(power))    Serial.println("Error reading power");
  else if (isnan(energy))   Serial.println("Error reading energy");
  else if (isnan(frequency))Serial.println("Error reading frequency");
  else if (isnan(pf))       Serial.println("Error reading power factor");
  else 
  { Serial.print("Voltage: ");    Serial.print(voltage);      Serial.println(" V");
    Serial.print("Current: ");    Serial.print(current);      Serial.println(" A");
    Serial.print("Power: ");      Serial.print(power);        Serial.println(" W");
    Serial.print("Energy: ");     Serial.print(energy, 3);    Serial.println(" kWh");
    Serial.print("Frequency: ");  Serial.print(frequency, 1); Serial.println(" Hz");
    Serial.print("PF: ");         Serial.println(pf);
  }

  Serial.println();
  delay(2000);
}
