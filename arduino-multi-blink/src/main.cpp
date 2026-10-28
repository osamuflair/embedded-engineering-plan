#include <Arduino.h>

const int LED_PIN_A = 12;
const int LED_PIN_B = 7;
const int LED_PIN_C = 4;
const unsigned long BLINK_INTERVAL_A_MS = 200;
const unsigned long BLINK_INTERVAL_B_MS = 500;
const unsigned long BLINK_INTERVAL_C_MS = 1000;
unsigned long previous_time_A = 0;
unsigned long previous_time_B = 0;
unsigned long previous_time_C = 0;
bool led_state_A = LOW;
bool led_state_B = LOW;
bool led_state_C = LOW;

void setup() {
  pinMode(LED_PIN_A, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);
  pinMode(LED_PIN_C, OUTPUT);
}

void loop() {
  unsigned long current_time = millis();
  if (current_time - previous_time_A >= BLINK_INTERVAL_A_MS){
    led_state_A = !led_state_A;
    digitalWrite(LED_PIN_A, led_state_A);
    previous_time_A = current_time;
  }
  if (current_time - previous_time_B >= BLINK_INTERVAL_B_MS){
    led_state_B = !led_state_B;
    digitalWrite(LED_PIN_B, led_state_B);
    previous_time_B = current_time;
  }
  if (current_time - previous_time_C >= BLINK_INTERVAL_C_MS){
    led_state_C = !led_state_C;
    digitalWrite(LED_PIN_C, led_state_C);
    previous_time_C = current_time;
  }
}