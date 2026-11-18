#include <Arduino.h>
#include <Wire.h>

int decode_seconds(int raw_seconds);
int decode_minutes(int raw_minutes);
int decode_hours(int raw_hours);
void print_two_digits(int value);

const int TARGET_ADDRESS = 0x68;
const int SECONDS_REGISTER = 0x00;
const int CLOCK_HALT_CLEAR_MASK = 0x7F;
const int ONES_MASK = 0x0F;
const int HOURS_12_MODE_MASK = 0x40;
const int TENS_SHIFT = 4;
const unsigned long READ_INTERVAL_MS = 500;
unsigned long previous_time = 0;
int previous_second = -1;

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
    int second;
    int minute;
    int hour;

    Wire.beginTransmission(TARGET_ADDRESS);
    Wire.write(SECONDS_REGISTER);
    if (Wire.endTransmission() == 0){
      Wire.requestFrom(TARGET_ADDRESS, 3);
      if (Wire.available() > 2){
        raw_seconds = Wire.read();
        raw_minutes = Wire.read();
        raw_hours = Wire.read();
        hour = decode_hours(raw_hours);
        minute = decode_minutes(raw_minutes);
        second = decode_seconds(raw_seconds);
        if (previous_second != second){
          if (hour == -1){
            Serial.println("12 HOURS MODE IS NOT SUPPORTED!!");
          }
          else{
            print_two_digits(hour);
            Serial.print(":");
            print_two_digits(minute);
            Serial.print(":");
            print_two_digits(second);
            Serial.println();
          }
          previous_second = second;
        }
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
  if (raw_hours & HOURS_12_MODE_MASK){
    return -1;
  }
  else{
    return (((raw_hours >> TENS_SHIFT) * 10) + (raw_hours & ONES_MASK));
  }
}

void print_two_digits(int value){
  if (value < 10){
    Serial.print("0");
  }
  Serial.print(value);
}