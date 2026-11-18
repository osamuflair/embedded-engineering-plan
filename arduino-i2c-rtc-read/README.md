# arduino-i2c-rtc-read

Reads the time (hours, minutes and seconds) from a DS1307 real-time clock over I²C using only the `Wire` library, decodes the clock's BCD format, and prints it as `HH:MM:SS`.

## What it does
The program asks the DS1307 for its time and prints it once per second:

```
22:07:58
22:07:59
22:08:00
22:08:01
```

If the clock does not answer, it prints `clock not found`. If the clock is in 12-hour mode, which this program does not decode, it prints `12 HOURS MODE IS NOT SUPPORTED!!` instead of a wrong time.

## How it works
The DS1307 is an I²C target at address `0x68`. Its data is stored in numbered registers, like pages in a notebook: seconds in `00h`, minutes in `01h` and hours in `02h`.

1. **Choose the starting register.** `beginTransmission(0x68)`, `write(0x00)`, then `endTransmission()`. The number `endTransmission()` returns is checked: 0 means the clock answered.
2. **Read three bytes in one go.** `requestFrom(0x68, 3)` returns the seconds, then the minutes, then the hours, because the clock's register pointer moves on by itself after each byte is read. `Wire.available()` confirms all three arrived before they are read.
3. **Decode each byte.** See below.
4. **Print only when the second changes**, with leading zeros from a small `print_two_digits()` helper.
5. **Time it.** The clock is read every 500 ms with a `millis()` timer, and no `delay()` is used.

### The register format
The clock stores each decimal digit in its own group of bits (BCD, binary coded decimal):

| Register | Bit 7 | Bit 6 | Bits 5 to 4 | Bits 3 to 0 |
|---|---|---|---|---|
| Seconds `00h` | CH (clock halt) | tens of seconds (bits 6 to 4) | | ones |
| Minutes `01h` | always 0 | tens of minutes (bits 6 to 4) | | ones |
| Hours `02h` | always 0 | 12 or 24-hour mode select | tens of hours (24-hour mode) | ones |

So 45 minutes is stored as `0100 0101`, which is `0x45`. Printed as an ordinary number, that byte is 69.

### The decode
- **Seconds:** clear bit 7 (the CH flag), shift right by 4 for the tens digit, mask the low 4 bits for the ones digit, then tens × 10 + ones.
- **Minutes:** the same, without the CH step.
- **Hours:** bit 6 selects the mode. If it is 0, the clock is in 24-hour mode and bits 5 to 4 are the tens digit, so the decode is the same as for minutes. If it is 1, the clock is in 12-hour mode, which is not supported, so the function returns `-1` and the main loop prints a clear message.

The decode functions only decode. They return a number, and the main loop decides what to print.

## Wiring
| Clock pin | Arduino pin |
|---|---|
| SDA | A4 |
| SCL | A5 |
| 5V | 5V |
| GND | GND |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code), with a DS1307 as the I²C target.

## Result
The time prints once per second as `HH:MM:SS` with leading zeros, and matches the real time. The seconds roll over from `59` to `00`. With the clock's SDA wire disconnected, the program prints `clock not found`.

## What I learned
- Reading an I²C register is two steps: write the register number, then request the data. The write has to sit inside the `beginTransmission` and `endTransmission` pair, or the register number never reaches the device.
- **Only the starting register is written.** In an I²C write, the first byte is the register number and any further byte is *data written into that register*. My first multi-register version wrote the value `1` into the seconds register on every pass, so the clock showed hours and minutes where I expected minutes and seconds. On real hardware, that would silently change the time.
- To read several registers, request several bytes. The register pointer advances by itself.
- `endTransmission()` returning 0 is the check that the device answered. Check it on every transaction.
- BCD stores each decimal digit in its own 4 bits, so a raw byte looks wrong when printed as an ordinary number. Decoding is a mask, a shift and a little arithmetic.
- Each field needs its own mask, matched to its own bits, and a variable's type has to be able to hold the value (a `bool` or a 16-bit `int` would each have broken something).
- **Sample faster than the signal changes.** Reading once a second, at the same rate as the clock, caused skipped seconds, because the read point slowly slid against the clock's ticks. Reading every 500 ms and printing only when the value changes fixed it.
- A configuration bit, such as the hours mode, has to be checked and not assumed. A clock set up by someone else could be in the other mode and give a wrong hour with no warning.
- Datasheet tables can be drawn ambiguously, so I confirmed the hours layout against the datasheet's text and against the real time on screen.
- Named constants and small helpers make the code read like the datasheet.

## Next
Show the time on an I²C LCD that shares the same two wires, which will make the scanner find two addresses.