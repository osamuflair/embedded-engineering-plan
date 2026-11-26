#include <Arduino.h>

const byte LED_PIN = 6;
const byte INTERRUPT_PIN = 2;
volatile byte state = LOW;
unsigned long previous_time = 0;
const unsigned long DEBOUNCE_TIME = 50;
volatile byte isr_counter = 0;
volatile byte accepted_presses = 0;
volatile byte last_isr_counter = 0;

void on_button_press();

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), on_button_press, FALLING);
}

void loop() {
  digitalWrite(LED_PIN, state);
  byte fired = isr_counter;
  if (last_isr_counter != fired) {
    byte accepted = accepted_presses;
    last_isr_counter = fired;
    Serial.print("Number of times interrupt has been fired: ");
    Serial.println(fired);
    Serial.print("Number of presses that passes debounce: ");
    Serial.println(accepted);
  }
}

void on_button_press() {
  isr_counter += 1;
  unsigned long current_time = millis();
  if (current_time - previous_time >= DEBOUNCE_TIME) {
    state = !state;
    previous_time = current_time;
    accepted_presses += 1;
  }
}