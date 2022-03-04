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
  hum = dht.readHumidity();
  
  //Caculate the wet value
  wetValue = analogRead(wetPin);
  wetValue *= 100;
  wetValue = wetValue/1023;
}

void temled(){
   if(dht.readTemperature()< 20){
     printByte(thermometerlow);
     delay(1000);
  }else if(dht.readTemperature() > 30){
     printByte(thermometerhigh);
     delay(1000);
  }else{
     printByte(smile);
     delay(1000);
  }
}
  
void wetled(){
   if(wetValue < 30){
     printByte(waterdropno);
     delay(1000);
  }else if(wetValue > 60){
     printByte(waterdrop);
     delay(1000);
  }else{
     printByte(smile);
     delay(1000);
  }
}

void loop() {
  getSensor();
  temled();
  wetled();
  delay(1000);
}
