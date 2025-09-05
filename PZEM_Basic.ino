/*!
 *@file       : PZEM-004T_ESP32.ino
 *@brief      : 100A PZEM-004T with ESP32
 *@copyright  : Copyright (c) 2022 MYBOTIC
 *@licence    : The MIT License (MIT)
 *@author     : Adapted for ESP32 by ChatGPT
 *@version    : V3.0
 *@date       : 03-09-2025
 */

#include <PZEM004Tv30.h>

// Define RX/TX pins for ESP32 (adjust to your wiring)
#define PZEM_RX 16   // GPIO16
#define PZEM_TX 17   // GPIO17
#define RLY1    23

PZEM004Tv30 pzem(Serial2, PZEM_RX, PZEM_TX);
int cnt=1;

void setup() 
{ pinMode(RLY1,OUTPUT);
  digitalWrite(RLY1,HIGH);
  Serial.begin(115200);

 // Initialize Serial2 (baud is fixed at 9600 for PZEM-004T)
  Serial2.begin(9600, SERIAL_8N1, PZEM_RX, PZEM_TX);
  Serial.println("PZEM-004T ESP32 Test");
}

void loop() {
  float voltage = pzem.voltage();
  if (!isnan(voltage)) {
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");
  } else {
    Serial.println("Error reading voltage");
  }

  float current = pzem.current();
  if (!isnan(current)) {
    Serial.print("Current: ");
    Serial.print(current);
    Serial.println(" A");
  } else {
    Serial.println("Error reading current");
  }

  float power = pzem.power();
  if (!isnan(power)) {
    Serial.print("Power: ");
    Serial.print(power);
    Serial.println(" W");
  } else {
    Serial.println("Error reading power");
  }

  float energy = pzem.energy();
  if (!isnan(energy)) {
    Serial.print("Energy: ");
    Serial.print(energy, 3);
    Serial.println(" kWh");
  } else {
    Serial.println("Error reading energy");
  }

  float frequency = pzem.frequency();
  if (!isnan(frequency)) {
    Serial.print("Frequency: ");
    Serial.print(frequency, 1);
    Serial.println(" Hz");
  } else {
    Serial.println("Error reading frequency");
  }

  float pf = pzem.pf();
  if (!isnan(pf)) {
    Serial.print("PF: ");
    Serial.println(pf);
  } else {
    Serial.println("Error reading power factor");
  }

  cnt++;
  if(cnt==5)  digitalWrite(RLY1,HIGH);
  if(cnt==10) digitalWrite(RLY1,LOW);
  if(cnt==11) cnt=1;

  Serial.print("----------------------------- ");
  Serial.println(cnt*2);
  delay(2000);
}
