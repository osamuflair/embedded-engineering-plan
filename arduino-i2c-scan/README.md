# arduino-i2c-scan

An I²C bus scanner. It finds every device that answers on the two I²C wires and prints its address over Serial.

## What it does
On start-up the Arduino tries every address from 1 to 127. Each device that answers is printed in hex, for example `0x68`. At the end it prints how many devices it found, or `No device found` if none answered.

With the DS1307 clock connected, the output is:
```
0x68
devices found: 1
```

## How it works
I²C is a bus where one controller (the Arduino) talks to many targets over two shared wires. Each target has its own 7-bit address, and only the one whose address is called answers.

1. `Wire.begin()` starts the I²C hardware.
2. A `for` loop visits every possible address.
3. For each address, `Wire.beginTransmission(address)` calls it and `Wire.endTransmission()` hangs up. The number returned is the check: **0 means a device answered**, and any other number means nobody did.
4. A responding address is printed with `Serial.print(address, HEX)`. Addresses below 16 get a leading `0`, so 8 prints as `0x08` like in datasheets.
5. A counter gives the final message.

The scan runs once, in `setup()`, so `loop()` is empty.

## Wiring
| Clock pin | Arduino pin |
|---|---|
| SDA | A4 |
| SCL | A5 |
| 5V | 5V |
| GND | GND |

SDA carries the data and SCL carries the clock beat. Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code), with a DS1307 real-time clock as the I²C target.

## Result
With the clock wired, the scanner prints `0x68` and reports one device. With the clock's SDA wire removed, it prints `No device found`, which shows the result comes from the real wiring.

## What I learned
- I²C uses two wires, SDA (data) and SCL (clock), shared by many devices. Each device has its own address, and the controller starts every conversation.
- Addresses are 7 bits, so there are 128 possible values. A few at each end are reserved for special use.
- `endTransmission()` returns 0 when a device acknowledged its address. That one check is the whole scanner.
- Normally the address comes from the datasheet, and the scanner is a setup and debugging tool. If it finds nothing, the problem is wiring or power, not the code.
- Real code should still check the result of every I²C call, so a missing device gives a clear message instead of silent bad data.
- Print addresses in hex with padding, so they match datasheets.
- Name constants for what they mean: the loop limit and the constant had to agree about whether 127 was included.

## Next
Read a register from the DS1307: send the register number, then request a byte, and decode the clock's BCD format into real seconds.