#include <Wire.h> 
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int wetPin = A2;     
long int wetValue = 0; 
  

void setup(){
    Serial.begin(9600);  
    //Initialise the DHT sensor
    dht.begin();
}

void loop(){
    getSensor();                   
    delay(1000);
}

void getSensor(){
  Serial.print("Temperature: ");  
  Serial.println(dht.readTemperature()); 
  Serial.print("Humidity: ");
  Serial.println(dht.readHumidity());  
  Serial.print("Soil moisture: ");
  Serial.println(wetValue); 
  Serial.println(); 
}
