# arduino-millis-blink

Two things at the same time on one Arduino, without `delay()`: an LED that blinks on its own, and an LED that follows a button instantly.

## What it does
- **LED A** (pin 12) blinks every 200 ms, driven by `millis()`.
- **LED B** (pin 13) turns on while the button (pin 2) is held and off when it is released.
- The button is checked on every pass through `loop()`, so it responds immediately, even in the middle of LED A's blink cycle.

## Why not `delay()`?
`delay()` freezes the board. In my first version (blink with `delay(1000)` twice per loop), the button was only read once every 2 seconds. Quick taps were missed, and LED B reacted up to 2 seconds late.

`millis()` returns the number of milliseconds since the board started. Instead of waiting, the loop runs continuously and asks: *has enough time passed since the last toggle?* If yes, toggle the LED. If no, carry on and check the button.

## How it works
1. `previous_time` remembers when LED A last changed.
2. On every pass, `current_time - previous_time` is compared with `BLINK_INTERVAL_MS`.
3. When the interval has passed, the LED state is flipped, written to the pin, and `previous_time` is updated.
4. The button is read on every pass, independent of the blink timing.

Using subtraction (`current_time - previous_time`) keeps the comparison correct even when `millis()` wraps around after about 49 days, because the variables are `unsigned long`.

## Wiring
| Part | Pin | Notes |
|---|---|---|
| LED A | 12 | with a series resistor (150 Ω) |
| LED B | 13 | with a series resistor (150 Ω) |
| Button | 2 | other side to GND, uses `INPUT_PULLUP` |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code)

## Result
- **With `delay()`** (blink with `delay(1000)` twice per loop): LED B took a very long time to come on after pressing the button. The board only read the button once per 2-second loop.
- **With `millis()`:** LED A blinks every 200 ms and the button is checked on every pass of `loop()`.
- LED B responds to a press immediately, even in the middle of LED A's blink.

## What I learned
- `delay()` blocks everything, `millis()` lets the program keep doing other work.
- To use `millis()`, remember the last time something happened and compare against it.
- Variables that change use lowercase names. Constants use UPPER_CASE.

## Next
Blink several LEDs at different rates at the same time.