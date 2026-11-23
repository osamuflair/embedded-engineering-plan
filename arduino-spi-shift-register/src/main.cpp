#include <Arduino.h>
#include <SPI.h>

void write_leds(byte pattern);

// Wiring (74HC595):
//   DS   -> pin 11 (MOSI, fixed by the Uno's SPI hardware)
//   SHCP -> pin 13 (SCK, fixed by the Uno's SPI hardware)
//   STCP -> pin 2  (latch, our choice)
const int LATCH_PIN = 2;
const unsigned long SPI_SPEED_HZ = 1000000;
const unsigned long WAIT_TIME = 300;

unsigned long previous_time = 0;
byte counter = 0;

void setup() {
  SPI.begin();
  pinMode(LATCH_PIN, OUTPUT);
  digitalWrite(LATCH_PIN, HIGH);
}

void loop() {
  unsigned long current_time = millis();
  if (current_time - previous_time >= WAIT_TIME) {
    write_leds(counter);
    counter += 1;
    previous_time = current_time;
  }
}

void write_leds(byte pattern) {
  SPI.beginTransaction(SPISettings(SPI_SPEED_HZ, MSBFIRST, SPI_MODE0));
  digitalWrite(LATCH_PIN, LOW);   // start: outputs stay unchanged while we shift
  SPI.transfer(pattern);          // shift 8 bits into the chip
  digitalWrite(LATCH_PIN, HIGH);  // latch: copy the 8 bits to the LEDs at once
  SPI.endTransaction();
}