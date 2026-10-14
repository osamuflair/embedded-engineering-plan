#include <Arduino.h>

const int LED_PIN = 3;
const int POTENTIOMETER_PIN = A1;
const int ANALOG_READ_MAX = 1023;
const int MAX_BRIGHTNESS = 255;

void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int potentiometer_value;
  int led_value;
  potentiometer_value = analogRead(POTENTIOMETER_PIN);
  led_value = (long(potentiometer_value) * MAX_BRIGHTNESS) / ANALOG_READ_MAX;
  analogWrite(LED_PIN, led_value);
}