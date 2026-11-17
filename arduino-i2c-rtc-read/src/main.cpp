#include <Arduino.h>
#include <Wire.h>

int decode_seconds(int raw_seconds);
int decode_minutes(int raw_minutes);
int decode_hours(int raw_hours);

const int TARGET_ADDRESS = 0x68;
const int SECONDS_REGISTER = 0x00;
//const int MINUTES_REGISTER = 0x01;
const int CLOCK_HALT_CLEAR_MASK = 0x7F;
const int ONES_MASK = 0x0F;
const int TENS_SHIFT = 4;
const unsigned long READ_INTERVAL_MS = 500;
unsigned long previous_time = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  unsigned long current_time = millis();
  if (current_time - previous_time >= READ_INTERVAL_MS){
    int raw_seconds;
    int raw_minutes;
    int raw_hours;
    Wire.beginTransmission(TARGET_ADDRESS);
    Wire.write(SECONDS_REGISTER);
    //Wire.write(MINUTES_REGISTER);
    if (Wire.endTransmission() == 0){
      Wire.requestFrom(TARGET_ADDRESS, 3);
      if (Wire.available() > 2){
        raw_seconds = Wire.read();
        raw_minutes = Wire.read();
        raw_hours = Wire.read();
        //Serial.print(raw_seconds);
        Serial.print(decode_hours(raw_hours));
        Serial.print(":");
        Serial.print(decode_minutes(raw_minutes));
        Serial.print(":");
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

int decode_minutes(int raw_minutes){
  return (((raw_minutes >> TENS_SHIFT) * 10) + (raw_minutes & ONES_MASK));
}

int decode_hours(int raw_hours){
  if ((raw_hours >> 6) & 1){
    return -1;
  }
  else{
    return (((raw_hours >> TENS_SHIFT) * 10) + (raw_hours & ONES_MASK));
  }
}