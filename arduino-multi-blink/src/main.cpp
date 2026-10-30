#include <Arduino.h>

const int LED_NUMBER = 3;
const int LED_PIN[LED_NUMBER] = {12, 7, 4};
const unsigned long BLINK_INTERVAL_MS[LED_NUMBER] = {200, 500, 1000};
unsigned long previous_time[LED_NUMBER] = {0, 0, 0};
bool led_state[LED_NUMBER] = {false, false, false};

void setup() {
  for (int i = 0; i < LED_NUMBER; i++){
    pinMode(LED_PIN[i], OUTPUT);
  }
}

void loop() {
  unsigned long current_time = millis();
  for (int i = 0; i < LED_NUMBER; i++){
    if (current_time - previous_time[i] >= BLINK_INTERVAL_MS[i]){
      led_state[i] = !led_state[i];
      digitalWrite(LED_PIN[i], led_state[i]);
      previous_time[i] = current_time;
    }
  }
}