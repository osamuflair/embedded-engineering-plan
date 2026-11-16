#include <Arduino.h>
#include <Wire.h>

int decode_seconds(int raw_seconds);

const int TARGET_ADDRESS = 0x68;
const int SECONDS_REGISTER = 0x00;
const int CLOCK_HALT_CLEAR_MASK = 0x7F;
const int ONES_MASK = 0x0F;
const int TENS_SHIFT = 4;
const unsigned long READ_INTERVAL_MS = 1000;
unsigned long previous_time = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  unsigned long current_time = millis();
  if (current_time - previous_time >= READ_INTERVAL_MS){
    int raw_seconds;
    Wire.beginTransmission(TARGET_ADDRESS);
    Wire.write(SECONDS_REGISTER);
    if (Wire.endTransmission() == 0){
      Wire.requestFrom(TARGET_ADDRESS, 1);
      if (Wire.available() > 0){
        raw_seconds = Wire.read();
        Serial.print(raw_seconds);
        Serial.print("    ");
        Serial.println(decode_seconds(raw_seconds));
      }
    }
    else{
      Serial.println("clock not found");
    }
    previous_time = current_time;
  }
}

int decode_seconds(int raw_seconds){
  raw_seconds &= CLOCK_HALT_CLEAR_MASK;
  return (((raw_seconds >> TENS_SHIFT) * 10) + (raw_seconds & ONES_MASK));
}