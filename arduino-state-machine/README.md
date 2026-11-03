# arduino-state-machine

One push button cycles an LED through four modes: off, steady on, slow blink and fast blink. It is a finite state machine, built without `delay()`.

## What it does
Each button press moves to the next mode, in this order:
1. **OFF**: the LED is off.
2. **ON**: the LED is steady on.
3. **SLOW_BLINK**: the LED toggles every 500 ms.
4. **FAST_BLINK**: the LED toggles every 150 ms.

After FAST_BLINK, the next press goes back to OFF. The Serial monitor prints `PRESSED` for each accepted press.

## How it works
The program is always in exactly one mode, held in a variable of an `enum Mode` type. `loop()` does two separate jobs:

1. **Deciding.** The button is debounced with `millis()` (50 ms), like in `arduino-button-debounce`. Each accepted press only changes the mode to the next one. It does not touch the LED.
2. **Doing.** On every pass through `loop()`, a `switch` on the current mode decides what the LED does:
   - OFF and ON write a fixed level to the pin.
   - SLOW_BLINK and FAST_BLINK share one block. The interval is chosen from the mode, and the LED toggles when that interval has passed.

The two jobs are kept apart because a blinking mode has to act between presses, and code that only runs on a press can't do that.

`led_state` is kept in step with the pin in every mode, so a blink mode always starts from the right state.

## Wiring
| Part | Pin | Notes |
|---|---|---|
| Button | 2 | other side to GND, `INPUT_PULLUP` (pressed reads LOW) |
| LED | 6 | pin → 1000 Ω resistor → LED anode, cathode → GND |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code). The pushbutton has `bounce` turned on in `diagram.json`.

## Result
Pressing the button steps through off, on, slow blink and fast blink, then wraps back to off. The blink rhythm does not hiccup when the button is pressed, and a blink mode always starts cleanly.

## What I learned
- A state machine is a variable that says which mode the program is in, plus rules for moving between modes. A "finite" state machine just means the list of modes is fixed.
- Separate deciding from doing: the press changes the mode, and a `switch` on every pass acts on it.
- An `enum` gives the modes names instead of numbers, which makes the code readable.
- Two jobs must not share one timer. When the debounce and the blink used the same variable, each reset the other and the blink rhythm stuttered.
- A state variable has to match the real pin. Whenever the pin is written a fixed level, the matching variable must be updated too, or the next blink starts wrong.
- Variables declared inside a `case` need braces, or later cases can fail to compile.
- Two modes that differ only by a number (the blink interval) can share one block.

## Next
Serial and UART: sending commands from the Serial monitor to the Arduino so I can choose the mode by typing, not just with the button.