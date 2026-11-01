#include <Arduino.h>

const int LED_PIN = 6;
const int BUTTON_PIN = 2;
const unsigned long DEBOUNCE_DELAY_MS = 50;
unsigned long previous_change_time = 0;
bool last_state = HIGH;
bool led_state = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  bool new_state = digitalRead(BUTTON_PIN);
  if (new_state != last_state){
    unsigned long current_time = millis();
    if (current_time - previous_change_time >= DEBOUNCE_DELAY_MS){
      last_state = new_state;
      previous_change_time = current_time;
      if (last_state == LOW){
        Serial.println("PRESSED");
        led_state = !led_state;
        digitalWrite(LED_PIN, led_state);
      }
      else{
        Serial.println("RELEASED");
      }
    }
  }
}