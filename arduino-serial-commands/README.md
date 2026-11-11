# arduino-serial-commands

The LED state machine from `arduino-serial-control`, now taking whole-word commands typed in the Serial monitor, as well as the push button.

## What it does
The LED has four modes: off, steady on, slow blink (500 ms) and fast blink (150 ms).

- **Typed word + Enter:** jumps straight to a named mode.
- **Button:** each press steps to the next mode in the cycle: off, on, slow, fast, then back to off.

| Word | Mode |
|---|---|
| `off` | OFF |
| `on` | ON |
| `slow` | SLOW_BLINK |
| `fast` | FAST_BLINK |

Words are not case-sensitive, so `slow`, `Slow` and `SLOW` all work. Any other word prints `invalid command`. Pressing Enter with nothing typed does nothing. Every accepted input prints the new mode.

## How it works
1. **Collecting.** Each byte that arrives is converted to upper case and stored in a character buffer. The program never waits for Enter, so the button keeps working while a word is half typed.
2. **Finishing.** When Enter (`\r` or `\n`) arrives and at least one character is stored, the buffer is closed with a `\0`, the position is reset, and the word is compared with `strcmp`.
3. **Acting.** A match sets `mode` directly. The button is debounced with `millis()` (50 ms) and moves `mode` to the next one in the cycle. A final `switch` on `mode` drives the LED on every pass.

Both inputs write the same `mode` variable, so the button always continues from wherever the last input left it.

### Buffer safety
The buffer holds `MAX_ARRAY_LENGTH` (6) characters, which leaves room for the `\0`. Extra characters beyond the limit are ignored. Because the buffer is bigger than the longest command (4 letters), a too-long word like `slowly` is stored as `SLOWL`, which can never match a real command, so it is reported as invalid instead of running by accident. If a longer command is ever added, the buffer must be enlarged first.

## Wiring
| Part | Pin | Notes |
|---|---|---|
| Button | 2 | other side to GND, `INPUT_PULLUP` (pressed reads LOW) |
| LED | 6 | pin → 1000 Ω resistor → LED anode, cathode → GND |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code). The pushbutton has `bounce` turned on in `diagram.json`.

## Result
Typing `off`, `on`, `slow` or `fast` and pressing Enter switches the LED to that mode. A too-long word and an unknown word are both rejected, an empty Enter does nothing, and the button works while I am typing.

## What I learned
- Text in C is a `char` array ending in `\0`, and `strcmp` compares two of them. `==` on text compares addresses, not contents.
- A word has to be collected one character at a time and only acted on when Enter arrives, because the bytes come in separately.
- A full buffer must never count as "the command is finished". My first version ran `slow` when I typed `slowly`.
- Overlong input needs a deliberate plan. Extra characters are ignored, so they can never write past the end of the array.
- Enter sends `\r` and/or `\n`, so an empty word must be ignored, or one Enter can report an error twice.
- Name a buffer size once and derive the limit from it, so the numbers can't drift apart.
- A `switch` only works on single values, so word commands need `strcmp` in an if-else chain.
- The Arduino `String` class is convenient, but a plain `char` array avoids memory problems on small boards.

## Next
I²C: reading a sensor over two wires.