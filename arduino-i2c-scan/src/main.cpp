#include <Arduino.h>
#include <Wire.h>

const int FIRST_ADDRESS = 1;
const int LAST_ADDRESS = 127;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  int device_count = 0;
  for(int address = FIRST_ADDRESS; address <= LAST_ADDRESS; address++ ){
    int error;
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0){
      Serial.print("0x");
      if (address < 16){
        Serial.print("0");
      }
      Serial.println(address, HEX);
      device_count++;
    }
  }
  if (device_count == 0){
    Serial.println("No device found");
  }
  else{
    Serial.print("devices found: ");
    Serial.println(device_count);
  }
}

void loop() {
}