# arduino-multi-blink

Three LEDs blinking at three different rates at the same time on one Arduino, without `delay()`.

## What it does
- **LED 0** (pin 12) blinks every 200 ms.
- **LED 1** (pin 7) blinks every 500 ms.
- **LED 2** (pin 4) blinks every 1000 ms.
- Each LED runs on its own timer, so none of them waits for another.

## How it works
The data for the LEDs lives in four arrays. The same index means the same LED in every array:
1. `LED_PIN[]` holds each LED's pin.
2. `BLINK_INTERVAL_MS[]` holds how often each LED toggles.
3. `previous_time[]` remembers when each LED last toggled.
4. `led_state[]` holds each LED's current state.

`LED_NUMBER` holds the number of LEDs in one place, and every loop uses it.

On every pass through `loop()`, the code reads `millis()` once. A `for` loop then visits each LED and asks: has `current_time - previous_time[i]` reached `BLINK_INTERVAL_MS[i]`? If yes, it toggles that LED and updates its `previous_time[i]`.

To add another LED, add one value to each array and change `LED_NUMBER`. The loop does not change.

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
- Name the count once (`LED_NUMBER`) and use it everywhere, so changing it never means hunting through the code.

## Next
The four arrays only work if their indexes stay lined up, and nothing warns me if they don't. Next I will group each LED's pin, interval, previous time and state into a struct, so one LED's data lives together.