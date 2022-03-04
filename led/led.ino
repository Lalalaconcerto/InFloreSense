// led.ino
// August Weinbren, Qian Jin, Xiaoling La
// This firmware was created to demonstrate the variety of graphics
// included with Infloresense.
//
// The following library was used: <https://github.com/wayoda/LedControl>
#include <LedControl.h>
int DIN = 10;
int CS = 9;
int CLK = 8;
LedControl lc = LedControl(DIN, CLK, CS, 0);
void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);  //Adjust the brightness maximum is 15
  lc.clearDisplay(0);
  // Happy facial expression--plant is in optimal condition
  byte smile[8] =  {0x00, 0x00, 0x24, 0x00, 0x24, 0x18, 0x00, 0x00};
  // in need of watering
  byte waterdrop[8] = {0x00, 0x08, 0x1c, 0x3e, 0x3e, 0x3e, 0x1c, 0x00};
  // soil is too moist
  byte waterdropno[8] = {0x01, 0x0a, 0x14, 0x2a, 0x32, 0x22, 0x5c, 0x80};
  // temperature needs to be increased
  byte thermometerhigh[8] = {0x32, 0x37, 0x32, 0x30, 0x30, 0x78, 0x78, 0x30};
  // temperature needs to be reduced
  byte thermometerlow[8] = {0x30, 0x37, 0x30, 0x30, 0x30, 0x78, 0x78, 0x30};
}
void loop() {
  //Demonstrating all of the predefined graphics
  printByte(smile);
  delay(1000);
  printByte(waterdrop);
  delay(1000);
  printByte(waterdropno);
  delay(1000);
  printByte(thermometerhigh);
  delay(1000);
  printByte(thermometerlow);
  delay(1000);
}
void printByte(byte character [])
{
  int i = 0;
  for (i = 0; i < 8; i++)
  {
    lc.setRow(0, i, character[i]);
  }
}
