#include <Arduino.h>

const int LED_PIN = 13;
const int SWITCH_PIN = 2;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(SWITCH_PIN) == LOW){
    digitalWrite(LED_PIN, HIGH);
  }
    
  else{
    digitalWrite(LED_PIN, LOW);
  }
}
