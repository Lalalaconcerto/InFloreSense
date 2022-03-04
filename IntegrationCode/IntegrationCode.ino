#include <LedControl.h>
#include <Wire.h> 
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int temp;
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
float baseVoltageAverage;
int baseVoltageMax;
bool userTouch = false;

  //Facial Expression
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

void assessBaseVoltageAverageMax() {
  int baseVoltageSum = 0; // The total voltage incrementally added in order to find an average voltage value
  int baseVoltageMax = 0;
  for (int i = 0; i < 20; i++) {
    int baseVoltageReading = analogRead(leafPin);
    baseVoltageSum += baseVoltageReading;
    if (baseVoltageReading > baseVoltageMax) {
      baseVoltageMax = baseVoltageReading;
    }
    delay(100);
  }
  baseVoltageAverage = baseVoltageSum / 20.0;
}

void setup() {
  Serial.begin(9600);  
  dht.begin();
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);  //Adjust the brightness maximum is 15
  lc.clearDisplay(0);
  delay(1000);
  assessBaseVoltageAverageMax();
}

// Visualises a byte array in an 8x8 bit graphic
void printByte(byte character []) {
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

void electricity(){
  voltage_val = analogRead(leafPin);
  Serial.println(voltage_val);
  float cutoffValue = (baseVoltageMax > 1.5 * baseVoltageAverage) ? baseVoltageMax * 1.1 : 1.5 * baseVoltageAverage;
  if (voltage_val > cutoffValue) { // 400 is a cutoff value we found to be consistent with massaging a plant leaf
    userTouch = true;
  }
  if (baseVoltageMax)
  delay(100);  
}

void loop() {
  getSensor();
  templed();
  wetled();
  electricity();
  delay(100);
}
