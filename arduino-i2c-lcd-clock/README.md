# arduino-i2c-lcd-clock

A clock on a 16×2 LCD. The Arduino reads the time from a DS1307 real-time clock over I²C and shows it as `HH:MM:SS`. Both devices share the same two wires.

## What it does
- Shows the time on the top row of the LCD, once a second, with leading zeros: `    22:08:07`.
- If the clock does not answer, the screen shows `clock not found`.
- If the clock is in 12-hour mode, which this program does not decode, the screen shows `unsupported!!!!` instead of a wrong time.
- The error messages stay still with no flicker, and disappear on their own when the problem is fixed.

## How it works
The DS1307 (address `0x68`) and the LCD's I²C chip (address `0x27`) are both on the same SDA and SCL lines. Each one only answers when its own address is called, so they never get in each other's way. I found the LCD's address by running my I²C scanner on this wiring, and it printed two addresses.

1. **Read the time.** Every 500 ms, the program writes the starting register number to the clock, then requests three bytes: seconds, minutes and hours. The decode functions turn the BCD bytes into plain numbers (see `arduino-i2c-rtc-read` for how).
2. **Draw it.** The time is written at a fixed column and row, always as eight characters. Because every character is overwritten every second, the old time never needs to be cleared.
3. **Handle problems with a display state.** The screen is always in one of three states: showing the time, showing "clock not found", or showing "unsupported". A function, `change_display_state()`, compares the new state with the current one. Only when they differ, it clears the screen, remembers the new state and tells the caller. The message is drawn once, at that moment. When the state is unchanged, the screen is left alone.

## Wiring
| Part | Pin |
|---|---|
| DS1307 SDA and LCD SDA | A4 |
| DS1307 SCL and LCD SCL | A5 |
| DS1307 and LCD power | 5V |
| DS1307 and LCD ground | GND |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code). The LCD part is a 16×2 in I²C mode, and the clock is a DS1307.

## Library
The LCD uses `marcoschwartz/LiquidCrystal_I2C`, listed in `platformio.ini` under `lib_deps`. The built-in `Wire` library doesn't need listing, but a third-party library does.

## Result
The time appears on the top row and ticks once a second with no flicker. Setting the clock's address to a wrong value (`0x69`) shows `clock not found` and it stays still, and changing it back brings the time back with no leftover characters. Forcing the 12-hour check to trigger shows `unsupported!!!!` the same way.

## What I learned
- Several I²C devices can share two wires, because each has its own address. The scanner shows all of them, and it is how I found the LCD's address.
- Third-party libraries have to be listed under `lib_deps`. Built-in ones such as `Wire` don't. Libraries with the same name can have different interfaces, so examples have to match the library in use.
- `lcd.print()` never starts a new line. It continues from the cursor, so `setCursor(column, row)` (column first, then row) is needed before every print.
- Printing does not erase old text. Writing a fixed-width string in the same place each time overwrites everything, and no clearing is needed.
- `lcd.clear()` wipes the whole screen, and calling it every update makes the screen flicker. Clear only when the screen switches between different content.
- Function parameters are copies. My first version of the state-change function updated a copy of the state, so the real variable never changed. The function should update the global variable itself.
- Detecting a change in state (current state compared with new state) is the same idea as my LED state machine, and it avoids redrawing what hasn't changed.
- A wrong address is the easy way to test the "device not found" path in a simulator, since a wire can't be pulled out mid-run.

## Next
SPI: the other common chip-to-chip bus, which uses more wires and is faster.