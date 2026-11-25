#include <Arduino.h>

const byte LED_PIN = 6;
const byte INTERRUPT_PIN = 2;
volatile byte state = LOW;
unsigned long previous_time = 0;
unsigned long debounce_time = 50;

void toggle_led();

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), toggle_led, FALLING);
}

void loop() {
  digitalWrite(LED_PIN, state);
}

void toggle_led() {
  unsigned long current_time = millis();
  if (current_time - previous_time >= debounce_time) {
    state = !state;
    previous_time = current_time;
  } 
}