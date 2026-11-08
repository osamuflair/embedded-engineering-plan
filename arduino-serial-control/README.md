# arduino-serial-control

The LED state machine from `arduino-state-machine`, now controlled two ways: with a push button and by typing single characters in the Serial monitor.

## What it does
The LED has four modes: off, steady on, slow blink (500 ms) and fast blink (150 ms).

- **Button:** each press steps to the next mode in the cycle: off, on, slow, fast, then back to off.
- **Typed character:** jumps straight to a named mode.

| Character | Mode |
|---|---|
| `0` | OFF |
| `1` | ON |
| `s` or `S` | SLOW_BLINK |
| `f` or `F` | FAST_BLINK |

Enter (`\r` and `\n`) is ignored. Any other character prints `invalid character`. Every accepted input prints the new mode, so the Serial monitor shows what the Arduino understood.

## How it works
Both inputs change the same `mode` variable. They don't have separate memories:
1. **Typed command.** If `Serial.available() > 0`, read one byte and `switch` on it. A command sets the mode directly.
2. **Button.** The button is debounced with `millis()` (50 ms), like in `arduino-button-debounce`. An accepted press moves `mode` to the next one in the cycle.
3. **Execution.** A final `switch` on `mode` decides what the LED does on every pass: OFF and ON write a fixed level, and the two blink modes share one block that picks its interval from the mode.

Because the mode is shared, the button always continues from wherever the last input left it. If I type `1` while in slow blink, the next press goes to slow blink again, not fast. A typed command ignores the cycle, and the button follows it.

No `delay()` is used, so both inputs stay responsive at all times.

## Wiring
| Part | Pin | Notes |
|---|---|---|
| Button | 2 | other side to GND, `INPUT_PULLUP` (pressed reads LOW) |
| LED | 6 | pin → 1000 Ω resistor → LED anode, cathode → GND |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code). The pushbutton has `bounce` turned on in `diagram.json`.

Note: the Wokwi serial monitor in VS Code sends each key the moment it is pressed, like a terminal, so only single characters can be typed. That's why the commands are one character long.

## Result
Typing `0`, `1`, `s` or `f` switches the LED to that mode immediately. Pressing the button steps through the cycle from the current mode. Both can be mixed freely, and the monitor prints each change.

## What I learned
- UART is a serial link where both sides must agree on the speed (baud rate), here 9600.
- Check `Serial.available() > 0` before `Serial.read()`. My first version used `== 1`, which stops reading forever if two bytes arrive before the next pass, because the bytes never leave the buffer.
- Enter sends `\r` and/or `\n`, so these need their own cases, or they get reported as invalid characters.
- A `default` case in a `switch` is where unknown input belongs.
- Two inputs can share one state variable. One "steps" (the button) and one "jumps" (the typed command), and the code that acts on the state doesn't change.
- Printing the new mode on every change makes the program easy to test.

## Next
Read whole-word commands from Serial, such as `slow` or `off`, by collecting characters in a buffer until Enter arrives. After that, I²C.