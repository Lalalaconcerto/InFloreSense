// LED_with_data
// August Weinbren, Xiaoling La, Qian Jin
//
// This software measures moisture and temperature, outputting an 8x8 graphic showing 
// whether they need to be dropped or raised for optimal plant health

#include <LedControl.h>
#include <Wire.h> 
#include "DHT.h"

// Plant monitor initialisation
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int temp;
int hum;
int wetPin = A2;     
long int wetValue = 0; 
// LED initialisation
int DIN = 10;
int CS = 9;
int CLK = 8;
LedControl lc = LedControl(DIN, CLK, CS, 0);
  // graphics for the the different indicators
  byte smile[8] =  {0x00, 0x00, 0x24, 0x00, 0x24, 0x18, 0x00, 0x00};
  //moisture
  byte waterdrop[8] = {0x00, 0x08, 0x1c, 0x3e, 0x3e, 0x3e, 0x1c, 0x00};
  byte waterdropno[8] = {0x01, 0x0a, 0x14, 0x2a, 0x32, 0x22, 0x5c, 0x80};
  //temperature
  byte thermometerhigh[8] = {0x32, 0x37, 0x32, 0x30, 0x30, 0x78, 0x78, 0x30};
  byte thermometerlow[8] = {0x30, 0x37, 0x30, 0x30, 0x30, 0x78, 0x78, 0x30};

  // optimal-range indicator values for determining smile
  bool goodTemperature = false;
  bool goodMoisture = false;

void setup() {
  Serial.begin(9600);  
  dht.begin();
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);  // Adjust the brightness maximum is 15
  lc.clearDisplay(0);
}

// Visualises a byte array in an 8x8 bit graphic
void printByte(byte character [])
{
  int i = 0;
  for (i = 0; i < 8; i++)
  {
    lc.setRow(0, i, character[i]);
  }
}

// Readings and conversions of sensors into usable data
void getSensor(){
  temp = dht.readTemperature();
  
  //Calculate the wet value
  wetValue = analogRead(wetPin);
  wetValue *= 100;
  wetValue = wetValue/1023;
}

// Converts the temperature reading into an LED symbol
void templed(){
   goodTemperature = false;
   if (dht.readTemperature()< 20){
     printByte(thermometerlow);
     delay(1000);
  } else if(dht.readTemperature() > 30){
     printByte(thermometerhigh);
     delay(1000);
  } else { // If the temperature and the moisture are both in good ranges, will return a smile 
     goodTemperature = true;
     if (goodMoisture) {
       printByte(smile);
       delay(1000);
     }
  }
}
  
void wetled(){
   goodMoisture = false;
   if (wetValue < 30) {
     printByte(waterdropno);
     delay(1000);
  } else if (wetValue > 60) {
     printByte(waterdrop);
     delay(1000);
  } else {
     goodMoisture = true;
     if (goodTemperature) {
       printByte(smile);
       delay(1000);
     }
  }
}

void loop() {
  getSensor();
  temled();
  wetled();
}
