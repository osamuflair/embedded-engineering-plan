#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

enum DisplayState{SHOWING_TIME, SHOWING_NOT_FOUND, SHOWING_UNSUPPORTED};

int decode_seconds(int raw_seconds);
int decode_minutes(int raw_minutes);
int decode_hours(int raw_hours);
void print_two_digits(int value);
bool change_display_state(DisplayState new_state );

const int LCD_ADDRESS = 0x27;
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;
const int TARGET_ADDRESS = 0x68;
const int SECONDS_REGISTER = 0x00;
const int CLOCK_HALT_CLEAR_MASK = 0x7F;
const int ONES_MASK = 0x0F;
const int HOURS_12_MODE_MASK = 0x40;
const int TENS_SHIFT = 4;
const int TIME_COLUMN = 4;
const int TIME_ROW = 0;
const unsigned long READ_INTERVAL_MS = 500;

unsigned long previous_time = 0;
int previous_second = -1;

DisplayState display_state = SHOWING_TIME;

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
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
            if (change_display_state(SHOWING_UNSUPPORTED)){
              lcd.setCursor(0, 0);
              lcd.print("unsupported!!!!");
            }
          }
          else{
            change_display_state(SHOWING_TIME);
            lcd.setCursor(TIME_COLUMN, TIME_ROW);
            print_two_digits(hour);
            lcd.print(":");
            print_two_digits(minute);
            lcd.print(":");
            print_two_digits(second);
          }
          previous_second = second;
        }
      }
    }
    else{
      if (change_display_state(SHOWING_NOT_FOUND)){
        lcd.setCursor(0, 0);
        lcd.print("clock not found");
      }
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
    lcd.print("0");
  }
  lcd.print(value);
}

bool change_display_state(DisplayState new_state){
  if (display_state != new_state){
    lcd.clear();
    display_state = new_state;
    previous_second = -1;
    return true;
  }
  return false;
}