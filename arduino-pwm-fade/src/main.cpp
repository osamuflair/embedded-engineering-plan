#include <Arduino.h>

const int LED_PIN = 3;
const int MAX_BRIGHTNESS = 255;
const int MIN_BRIGHTNESS = 0;
const int FADE_STEP = 5; // must divide MAX_BRIGHTNESS evenly, or the fade never reaches the top
const int STEP_DELAY_MS = 30;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  for(int brightness = MIN_BRIGHTNESS; brightness <= MAX_BRIGHTNESS; brightness += FADE_STEP){
    analogWrite(LED_PIN, brightness);
    delay(STEP_DELAY_MS);
  }
  for(int brightness = MAX_BRIGHTNESS - FADE_STEP; brightness >= MIN_BRIGHTNESS + FADE_STEP; brightness -= FADE_STEP){
    analogWrite(LED_PIN, brightness);
    delay(STEP_DELAY_MS);
  }
}