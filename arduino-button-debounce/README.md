# arduino-button-debounce

A push button that toggles an LED on each press, with contact bounce filtered out so one press flips the LED exactly once.

## What it does
- Press the button once and the LED turns on. Press it again and the LED turns off.
- One physical press always gives exactly one change, even though the button's contacts bounce.
- Serial output prints `PRESSED` or `RELEASED` for each accepted change.

## The problem: contact bounce
When a button is pressed, its metal contacts vibrate for a few milliseconds before settling. The Arduino reads the pin thousands of times per second, so it sees that flicker as many separate presses.

I proved it first with a version that only counted changes. With the simulator's bounce turned on, one press printed 8 `PRESSED` and `RELEASED` pairs.

## How it works
1. Read the button every pass through `loop()`.
2. If the reading differs from the last accepted state, check how long it has been since the last accepted change.
3. If at least `DEBOUNCE_DELAY_MS` (50 ms) has passed, accept the change: save the new state and the time.
4. On an accepted press (the reading goes LOW), flip the LED.
5. A change that arrives inside the window is not accepted yet. The code sees it again on the next pass, and accepts it once the window has passed, if the button is still in that state.

No `delay()` is used, so the loop never blocks.

## Wiring
| Part | Pin | Notes |
|---|---|---|
| Button | 2 | other side to GND, `INPUT_PULLUP` (pressed reads LOW) |
| LED | 6 | pin → 1 kΩ resistor → LED anode, cathode → GND |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code). The pushbutton has `bounce` turned on in `diagram.json` so the simulation behaves like a real button.

## Result
One press flips the LED once, whether I press slowly or tap fast. The Serial monitor shows exactly one `PRESSED` and one `RELEASED` per press.

## What I learned
- A button's contacts bounce, and the Arduino is fast enough to read every flicker as a separate event.
- Reading the level is fine for "LED follows button". Toggling needs *events*, so bounce becomes a real bug.
- Debouncing means ignoring changes until enough time has passed since the last accepted one, using `millis()` and no `delay()`.
- Update the saved state only when a change is accepted, so a change inside the window isn't lost.
- In Wokwi, Serial output appears in the simulator's own serial monitor. PlatformIO's monitor on COM4 shows nothing without a physical board.
- A longer debounce time is safer for worn buttons, and human presses are much longer than 50 ms, so nobody will notice.

## Limits and next
This is a lockout: the first edge is trusted and later ones are ignored for a short time. A noise spike on a real wire could count as a press. Next I'll try a stricter version that restarts the timer on every flicker and accepts a change only after the signal has stayed steady for the full wait.