// test_electricty
// August Weinbren, Xiaoling La, Qian Jin
//
// This software tests the voltage outputs of a plant when connected to the sensor via a leaf.

uint8_t leafPin = 0;
int voltage_val;
int counter;
int baseCounter = 1;
float baseVoltageAverage; // The average voltage found over the initial 20 readings
int baseVoltageMax;
bool userTouch = false;

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
  Serial.begin(115200);
  delay(1000);
  assessBaseVoltageAverageMax();
}

void loop() {
  voltage_val = analogRead(leafPin);
  Serial.println(voltage_val);
  float cutoffValue = (baseVoltageMax > 1.5 * baseVoltageAverage) ? baseVoltageMax * 1.1 : 1.5 * baseVoltageAverage;
  if (voltage_val > cutoffValue) { // 400 is a cutoff value we found to be consistent with massaging a plant leaf
    userTouch = true;
  }
  if (baseVoltageMax)
  delay(100);  
}
