#include <LedControl.h>
#include <Wire.h> 
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float temp;
int hum;
int wetPin = A2;     
long int wetValue = 0; 
int DIN = 10;
int CS = 9;
int CLK = 8;
LedControl lc = LedControl(DIN, CLK, CS, 0);

uint8_t leafPin = 0;
int voltage_val;
int counter;
int baseCounter = 1;
int baseVoltage;
int baseVoltageSum = 0;
bool userTouch = false;

  //Facial Expression
  byte smile[8] =  {0x00, 0x00, 0x24, 0x00, 0x24, 0x18, 0x00, 0x00};
  //moisture
  byte waterdrop[8] = {0x00, 0x08, 0x1c, 0x3e, 0x3e, 0x3e, 0x1c, 0x00};
  // byte waterdropkong[8] = {0x00, 0x08, 0x14, 0x22, 0x22, 0x22, 0x1c, 0x00};
  byte waterdropno[8] = {0x01, 0x0a, 0x14, 0x2a, 0x32, 0x22, 0x5c, 0x80};
  //temperature
  byte thermometerhigh[8] = {0x32, 0x37, 0x32, 0x30, 0x30, 0x78, 0x78, 0x30};
  byte thermometerlow[8] = {0x30, 0x37, 0x30, 0x30, 0x30, 0x78, 0x78, 0x30};

void setup() {
  Serial.begin(9600);  
  dht.begin();
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);  //Adjust the brightness maximum is 15
  lc.clearDisplay(0);
}


void printByte(byte character [])
{
  int i = 0;
  for (i = 0; i < 8; i++)
  {
    lc.setRow(0, i, character[i]);
  }
}

void getSensor(){
  temp = dht.readTemperature();
  //Caculate the wet value
  wetValue = analogRead(wetPin);
  wetValue *= 100;
  wetValue = wetValue/1023;
}

void temled(){
   if(dht.readTemperature()< 20){
     printByte(thermometerlow);
  }else if(dht.readTemperature() > 30){
     printByte(thermometerhigh);
  }else{
     printByte(smile);
  }
}
  
void wetled(){
   if(wetValue < 30){
     printByte(waterdropno);
  }else if(wetValue > 60){
     printByte(waterdrop);
  }else{
     printByte(smile);
  }
}

void electricity(){
  counter++;
  if (baseCounter < 20){
    baseVoltageSum += analogRead(leafPin);
    baseCounter++;
  }
  else if (baseCounter == 20){
    baseVoltage = baseVoltageSum / 20;
    Serial.print("Base voltage: ");
    Serial.println(baseVoltage);
    baseCounter++;
  }
  else {
    voltage_val = analogRead(leafPin);
    Serial.println(voltage_val);
    if (voltage_val > 400) { // This is the value we found experimentally to be consistent with massaging a plant leaf
      userTouch = true;
    }
  }
}

void loop() {
  getSensor();
  temled();
  wetled();
  electricity();
  delay(100);
}
