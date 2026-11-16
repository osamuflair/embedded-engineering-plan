# arduino-i2c-rtc-read

Reads the seconds from a DS1307 real-time clock over I²C using only the `Wire` library, and decodes the clock's BCD format into real seconds.

## What it does
Once a second, the program asks the DS1307 for its seconds register and prints two numbers: the raw byte as the clock sent it, and the decoded seconds.

```
88    58
89    59
0     0
1     1
```

If the clock does not answer, it prints `clock not found`.

## How it works
The DS1307 is an I²C target at address `0x68`. Its data is stored in numbered registers, like pages in a notebook. The seconds are in register `0x00`.

1. **Choose the register.** `beginTransmission(0x68)`, `write(0x00)`, then `endTransmission()`. The number `endTransmission()` returns is checked: 0 means the clock answered.
2. **Read it.** `requestFrom(0x68, 1)` asks for one byte, and `Wire.available()` confirms it arrived before it is read.
3. **Decode it.** See below.
4. **Time it.** A `millis()` timer does this once per second. No `delay()` is used.

### The register format
The clock stores each decimal digit in its own group of bits (BCD, binary coded decimal):

| Bit 7 | Bits 6 to 4 | Bits 3 to 0 |
|---|---|---|
| CH (clock halt) | tens of seconds | ones of seconds |

So 45 seconds is stored as `0100 0101`, which is `0x45`. Printed as an ordinary number, that byte is 69. That is why the raw column looks wrong.

### The decode
`decode_seconds()` turns the raw byte into real seconds:
1. Clear bit 7 (`CLOCK_HALT_CLEAR_MASK`), so the CH flag cannot corrupt the seconds.
2. Shift right by 4 (`TENS_SHIFT`) to get the tens digit.
3. Mask the lowest 4 bits (`ONES_MASK`) to get the ones digit.
4. Combine them: tens × 10 + ones.

A raw byte of `0x59` (89) decodes to 59, and `0xA0` (CH set, 20 seconds) decodes to 20.

## Wiring
| Clock pin | Arduino pin |
|---|---|
| SDA | A4 |
| SCL | A5 |
| 5V | 5V |
| GND | GND |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code), with a DS1307 as the I²C target.

## Result
The decoded column counts up one per second and rolls over from 59 to 0. The raw column jumps where BCD and decimal disagree: at 39 to 40 seconds it goes from 57 to 64. With the clock's SDA wire disconnected, the program prints `clock not found`.

## What I learned
- Reading an I²C register is two steps: write the register number, then request the data. The write has to sit inside the `beginTransmission` and `endTransmission` pair, or the register number never reaches the device.
- `endTransmission()` returning 0 is the check that the device answered. Check it on every transaction.
- Datasheet register maps show which bits mean what. Labels such as "10 Seconds" mean the tens digit.
- BCD stores each decimal digit in its own 4 bits, so the raw byte looks wrong when printed as a plain number. Decoding is a mask, a shift and a little arithmetic.
- Each field needs its own mask, matched to its own bits. My first version used one mask for everything and counted the tens digit twice.
- A variable's type has to be able to hold the value. A `bool` would have turned every byte into 1, and an `int` would have overflowed `millis()` after about 33 seconds.
- Printing a value in hex is a quick way to look at a byte, but the program itself needs the real decoded number to compare or calculate with.
- A named constant for each mask and register makes the code read like the datasheet.

## Next
Read the minutes and hours as well, then show the time on an I²C LCD that shares the same two wires.