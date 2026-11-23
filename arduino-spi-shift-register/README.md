# arduino-spi-shift-register

An 8-bit binary counter on 8 LEDs. The Arduino sends one byte over SPI to a 74HC595 shift register, and the chip's eight outputs drive the LEDs.

## What it does
- Counts from 0 to 255 in binary on the 8 LEDs, one step every 300 ms.
- After 255 it wraps back to 0 on its own and starts again.

## How it works
SPI uses fixed pins on the Uno: MOSI on 11 and SCK on 13. The chip has no address, because it is the only device on the bus, so there is no chip-select line to call it. The latch pin is my own choice (pin 2).

1. **Send the byte.** `write_leds()` begins an SPI transaction, pulls the latch low, sends the byte with `SPI.transfer()`, pulls the latch high, then ends the transaction.
2. **Shift, then latch.** The 74HC595 has two stages. The 8 bits are clocked into a shift register one by one. The latch pulse (STCP) then copies all 8 bits to the outputs at once. [FILL IN: what would you see on the LEDs without the latch?]
3. **Time the counting.** `loop()` checks `millis()` and, every 300 ms, sends the counter and adds 1. The counter is a `byte`, so it wraps from 255 to 0 without any extra code.

## Wiring
| 74HC595 pin | Connected to |
|---|---|
| DS | pin 11 (MOSI) |
| SHCP | pin 13 (SCK) |
| STCP | pin 2 (latch) |
| MR | 5V (active low, so it must be held high) |
| OE | GND (active low, so outputs are always enabled) |
| VCC / GND | 5V / GND |
| Q0 to Q7 | the 8 LEDs, each through a 220 Ω resistor to GND |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code). SPI is built in, so `lib_deps` isn't needed.

## Result
The LEDs count up in binary. At count 5 the lit LEDs are [FILL IN: which ones], so bit 0 is [FILL IN: led1 or led8].

## What I learned
- On the Uno the SPI data and clock pins are fixed by hardware (11 and 13). My first wiring had them on the wrong pins, so nothing would have lit.
- Active-low pins such as MR and OE must be tied to a fixed level, or the chip may not work.
- The latch pin is under my control. The order matters: latch low, transfer, latch high.
- `SPISettings` fixes the speed, bit order and mode, and `beginTransaction` and `endTransaction` always come as a pair.
- Counting needs a counter variable that lives outside `loop()`. A `for` loop inside `loop()` ran all 256 values in a blink.
- Moving the timer out of `write_leds()` left one job per function: `write_leds()` sends, and `loop()` decides when.

## Next
Timers and interrupts.