uint8_t leafPin = 0;
int voltage_val;
int counter;
int baseCounter = 1;
int baseVoltage;
int baseVoltageSum = 0;
bool userTouch = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
counter++;
if (baseCounter < 20) {  
  baseVoltageSum += analogRead(leafPin);
  baseCounter++;
}
else if (baseCounter == 20) {
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
delay(100);
  // put your main code here, to run repeatedly:
  
}
