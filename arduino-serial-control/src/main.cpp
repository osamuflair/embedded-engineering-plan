#include <Arduino.h>

enum Mode{OFF, SLOW_BLINK, FAST_BLINK, ON};
const int LED_PIN = 6;
const int BUTTON_PIN = 2;
const unsigned long DEBOUNCE_DELAY_MS = 50;
const unsigned long SLOW_BLINK_MS = 500;
const unsigned long FAST_BLINK_MS = 150;
unsigned long previous_change_time = 0;
unsigned long previous_toggle_time = 0;
bool last_button_state = HIGH;
bool led_state = false;
Mode mode = OFF;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  //typed command
  if (Serial.available() > 0){
    int input_char = Serial.read();
    switch (input_char){
      case '0':
        Serial.println("0-OFF");
        mode = OFF;
        break;
      case '1':
        Serial.println("1-ON");
        mode = ON;
        break;
      case 's':
      case 'S':
        Serial.println("S-SLOW_BLINK");
        mode = SLOW_BLINK;
        break;
      case 'f':
      case 'F':
        Serial.println("F-FAST_BLINK");
        mode = FAST_BLINK;
        break;
      case '\r':
      case '\n':
        break;
      default:
        Serial.println("invalid character");
        break;
    }
  }
  
  //button
  bool new_button_state = digitalRead(BUTTON_PIN);
  if (new_button_state != last_button_state){
    unsigned long current_time = millis();
    if (current_time - previous_change_time >= DEBOUNCE_DELAY_MS){
      last_button_state = new_button_state;
      previous_change_time = current_time;
      if (last_button_state == LOW){
        switch (mode){
          case OFF:
            Serial.println("PRESSED-ON");
            mode = ON;
            break;
          case ON:
            Serial.println("PRESSED-SLOW_BLINK");
            mode = SLOW_BLINK;
            break;
          case SLOW_BLINK:
            Serial.println("PRESSED-FAST_BLINK");
            mode = FAST_BLINK;
            break;
          case FAST_BLINK:
            Serial.println("PRESSED-OFF");
            mode = OFF;
            break;
        }
      }
    }
  }

  //execution
  switch (mode){
    case OFF:
      led_state = false;
      digitalWrite(LED_PIN, LOW);
      break;
    case ON:
      led_state = true;
      digitalWrite(LED_PIN, HIGH);
      break;
    case SLOW_BLINK:
    case FAST_BLINK:
    {
      unsigned long blink_ms;
      if (mode == SLOW_BLINK){
        blink_ms = SLOW_BLINK_MS;
      }
      else{
        blink_ms = FAST_BLINK_MS;
      }
      unsigned long current_time = millis();
      if (current_time - previous_toggle_time >= blink_ms){
        led_state = !led_state;
        previous_toggle_time = current_time;
        digitalWrite(LED_PIN, led_state);
      }
      break;
    }
  }
}