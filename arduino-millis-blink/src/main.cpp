#include <Arduino.h>

const int LED_PIN_A = 12;
const int LED_PIN_B = 13;
const int BUTTON_PIN = 2;
const unsigned long BLINK_INTERVAL_MS = 200;
unsigned long previous_time = 0;
bool led_state = LOW;

void setup() {
  pinMode(LED_PIN_A, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  unsigned long current_time =  millis();
  if (current_time - previous_time >= BLINK_INTERVAL_MS){
    Serial.println(current_time);
    led_state = !led_state;
    digitalWrite(LED_PIN_A, led_state);
    previous_time = current_time;
  }

  if (digitalRead(BUTTON_PIN) == LOW){
    digitalWrite(LED_PIN_B, HIGH);
  }
  else{
    digitalWrite(LED_PIN_B, LOW);
  }
}