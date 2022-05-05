#include <LedControl.h>
#include <Wire.h>
#include "DHT.h"
#include "Adafruit_MPR121.h"
#include <SPI.h>
#include <ezTime.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

#define DHTPIN 4
#define DHTTYPE DHT22
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif


DHT dht(DHTPIN, DHTTYPE);
int temp;
int hum;
int wetPin = A2;
long int wetValue = 0;
int DIN = 10;
int CS = 9;
int CLK = 8;
bool LEDON = false; //user touch
int loopCounter = 0;
int secondsSinceLastTouch = 5;
LedControl lc = LedControl(DIN, CLK, CS, 0);
// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

bool userTouch = false;
bool goodTemperature = false;
bool goodMoisture = false;
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

int status = WL_IDLE_STATUS;     // the WiFi radio's status
const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;
const char* mqtt_server = "MQTT_SERVER";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
Timezone GB;

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
  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  Serial.println("Infloresense");
  delay(100);
  dht.begin();
  startWifi();
  Serial.println("Wifi started");
  syncDate();
  Serial.println("Date synced");
  client.setServer(mqtt_server, 1884);
  Serial.println("Server set");
  client.setCallback(callback);
  Serial.println("callback set");

  //mpr121
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

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

void getSensor() {
  temp = dht.readTemperature();
  //Caculate the wet value
  wetValue = analogRead(wetPin);
  wetValue *= 100;
  wetValue = wetValue / 1023;
}

void temled() {
  goodTemperature = false;
    if (dht.readTemperature() < 20) {
      printByte(thermometerlow);
    } else if (dht.readTemperature() > 30) {
      printByte(thermometerhigh);
    } else {
      goodTemperature = true;
      if (goodMoisture) {
        printByte(smile);
      }
    }
}

void wetled() {
  goodMoisture = false;
    if (wetValue < 30) {
      printByte(waterdropno);
    } else if (wetValue > 60) {
      printByte(waterdrop);
    } else {
      goodMoisture = true;
      if (goodTemperature) {
        printByte(smile);
      }
    }


}

void mpr121() {
  // Get the currently touched pads
  bool released = false;
 currtouched = cap.touched();
//  for (uint8_t i = 0; i < 12; i++) {
    if ((currtouched & _BV(0)) && !(lasttouched & _BV(0)))  {
      Serial.print(0); Serial.println(" touched");
      LEDON = 1;
    } else {
      LEDON = 0;
    }
    // if it *was* touched and now *isnt*, alert!
//    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
//      Serial.print(i); Serial.println(" released");
//      released = true;
//    }
//    if (i == 11 && released) {
//      LEDON = 0;
//    }
//  }

  // reset our state
//  lasttouched = currtouched;

  // comment out this line for detailed data from the sensor!
  return;

  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
//  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.filteredData(0)); Serial.print("\t");
//  }
  Serial.println();
  Serial.print("Base: ");
//  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.baselineData(0)); Serial.print("\t");
//  }
  Serial.println();
}


void startWifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // check to see if connected and wait until you are
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// This function is used to make sure the arduino is connected
// to an MQTT broker before it tries to send a message and to
// keep alive subscriptions on the broker (ie listens for inTopic)


void syncDate() {
  // get real date and time
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());

}



void sendMQTT() {
  // try to reconnect if not
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  

  snprintf (msg, 50, "%.0i", wetValue);
  Serial.print("Moisture: ");
  Serial.println(msg);
  client.publish("student/CASA0021/InfloreSense/Moisture", msg);

  snprintf (msg, 50, "%.0i", temp);
  Serial.print("Temp: ");
  Serial.println(msg);
  client.publish("student/CASA0021/InfloreSense/Temp", msg);

  snprintf (msg, 50, "%.0i", LEDON);
  Serial.print("LEDON: ");
  Serial.println(msg);
  client.publish("student/CASA0021/InfloreSense/LEDON", msg);
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {    // while not (!) connected....
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      // ... and subscribe to messages on broker
      client.subscribe("student/CASA0016/inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


// Execute code when a message is recieved from the MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void loop() {
    getSensor();
    bool LEDONBEFORE = LEDON;
    if (secondsSinceLastTouch > 4) {
      Serial.println("running MPR check; secondsSinceLastTouch:");
      Serial.println(secondsSinceLastTouch);
      mpr121();
    }
    if (LEDON && !LEDONBEFORE) {
      secondsSinceLastTouch = 0;
    } else if (LEDONBEFORE) {
      secondsSinceLastTouch++;
    }
    sendMQTT();
    if (LEDON) { //alternate between temp and moisture display
      Serial.print("loop counter:");
      Serial.println(loopCounter);
      loopCounter++;
      if (loopCounter == 2) {
        loopCounter = 0;
      }
      if (loopCounter == 0) {
        temled();
      } else {
        wetled();
      }
    } else {
      lc.clearDisplay(0);
    }

    Serial.println(GB.dateTime("H:i:s")); // UTC.dateTime("l, d-M-y H:i:s.v T")
    delay(1000);

}
