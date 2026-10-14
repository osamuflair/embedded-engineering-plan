#include <Arduino.h>

const int LED_PIN = 3;
const int POTENTIOMETER_PIN = A1;

void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int potentiometer_value;
  int led_value;
  potentiometer_value = analogRead(POTENTIOMETER_PIN);
  led_value = (long(potentiometer_value) * 255) / 1023;
  analogWrite(LED_PIN, led_value);
}