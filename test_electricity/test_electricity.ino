// test_electricty
// August Weinbren, Xiaoling La, Qian Jin
//
// This software tests the voltage outputs of a plant when connected to the sensor via a leaf.

uint8_t leafPin = 0;
int voltage_val;
int counter;
int baseCounter = 1;
int baseVoltage; // The average voltage found over the initial 20 readings
bool userTouch = false;
void setup() {
  Serial.begin(115200);
  delay(1000);
  int baseVoltageSum = 0; // The total voltage incrementally added in order to find an average voltage value
  for (int i = 0; i < 20; i++) {
    baseVoltageSum += analogRead(leafPin); 
    delay(100);
  }
  baseVoltage = baseVoltageSum / 20;
}

void loop() {
  voltage_val = analogRead(leafPin);
  Serial.println(voltage_val);
  if (voltage_val > 400) { // This is a cutoff value we found to be consistent with massaging a plant leaf
    userTouch = true;
  }
  delay(100);  
}
