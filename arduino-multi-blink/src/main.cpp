#include <Arduino.h>

struct Led
{
  const int pin;
  const unsigned long interval_ms;
  unsigned long previous_time;
  bool state;
};

Led LEDS[] = {
  {12, 200, 0, false},
  {7, 500, 0, false},
  {4, 1000, 0, false}
};

const int LED_NUMBER = sizeof(LEDS) / sizeof(LEDS[0]);

void setup() {
  for (int i = 0; i < LED_NUMBER; i++){
    pinMode(LEDS[i].pin, OUTPUT);
  }
}

void loop() {
  unsigned long current_time = millis();
  for (int i = 0; i < LED_NUMBER; i++){
    Led &led = LEDS[i];
    if (current_time - led.previous_time >= led.interval_ms){
      led.state = !led.state;
      digitalWrite(led.pin, led.state);
      led.previous_time = current_time;
    }
  }
}