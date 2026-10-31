# arduino-multi-blink

Three LEDs blinking at three different rates at the same time on one Arduino, without `delay()`.

## What it does
- **LED 0** (pin 12) blinks every 200 ms.
- **LED 1** (pin 7) blinks every 500 ms.
- **LED 2** (pin 4) blinks every 1000 ms.
- Each LED runs on its own timer, so none of them waits for another.

## How it works
Each LED is described by one `Led` struct that keeps all of its data together:
1. `pin`: which pin the LED is on.
2. `interval_ms`: how often it toggles.
3. `previous_time`: when it last toggled.
4. `state`: whether it is currently on or off.

All the LEDs live in one array, `LEDS[]`. The number of LEDs is computed automatically with `sizeof(LEDS) / sizeof(LEDS[0])`, so there is no count to keep up to date by hand.

On every pass through `loop()`, the code reads `millis()` once. A `for` loop then visits each LED, using a reference (`Led &led`) to the current one, and asks: has `current_time - led.previous_time` reached `led.interval_ms`? If yes, it toggles that LED and updates its `previous_time`.

To add another LED, add one line to `LEDS[]`. Nothing else changes.

## Wiring
| Part | Pin | Notes |
|---|---|---|
| LED 0 | 12 | pin → 150 Ω resistor → LED anode |
| LED 1 | 7 | pin → 150 Ω resistor → LED anode |
| LED 2 | 4 | pin → 150 Ω resistor → LED anode |

All three LED cathodes are connected together and go to GND.

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code)

## Result
All three LEDs blink at their own speeds at the same time, drifting in and out of step with each other.

## What I learned
- Use `>=`, not `==`, when checking whether an interval has passed. With `==` the check can be missed if a pass through `loop()` skips the exact millisecond, and then the LED would never toggle again.
- An LED only lights in one direction: pin → resistor → anode, and cathode → GND. My first wiring had it backwards and none of the LEDs lit.
- An array plus a `for` loop replaces repeated code. My first version copied the same block three times, and the array version does the same job with one block.
- A struct keeps one LED's data together. With separate arrays, a value in the wrong slot gives no error and the LEDs just misbehave.
- `sizeof(array) / sizeof(array[0])` gives the number of items, so adding an LED doesn't mean updating a count.
- A reference (`Led &led = LEDS[i]`) names the current item once instead of repeating `LEDS[i]`.

## Next
Button debouncing: a mechanical button can register one press as many, so I need to filter out the noise without using `delay()`.