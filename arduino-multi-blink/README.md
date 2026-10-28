# arduino-multi-blink

Three LEDs blinking at three different rates at the same time on one Arduino, without `delay()`.

## What it does
- **LED A** (pin 12) blinks every 200 ms.
- **LED B** (pin 7) blinks every 500 ms.
- **LED C** (pin 4) blinks every 1000 ms.
- Each LED runs on its own timer, so none of them waits for another.

## How it works
Each LED has its own copy of the pattern from `arduino-millis-blink`:
1. A constant for its interval (`BLINK_INTERVAL_A_MS`, and so on).
2. A variable that remembers when it last toggled (`previous_time_A`).
3. A variable that holds its current state (`led_state_A`).

On every pass through `loop()`, the code reads `millis()` once, then checks each LED separately: has `current_time - previous_time` reached its interval? If yes, toggle that LED and update its `previous_time`.

## Wiring
| Part | Pin | Notes |
|---|---|---|
| LED A | 12 | pin → 150 Ω resistor → LED anode |
| LED B | 7 | pin → 150 Ω resistor → LED anode |
| LED C | 4 | pin → 150 Ω resistor → LED anode |

All three LED cathodes are connected together and go to GND.

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code)

## Result
All three LEDs blink at their own speeds at the same time, drifting in and out of step with each other.

## What I learned
- Use `>=`, not `==`, when checking whether an interval has passed. With `==` the check can be missed if a pass through `loop()` skips the exact millisecond, and then the LED would never toggle again.
- An LED only lights in one direction: pin → resistor → anode, and cathode → GND. My first wiring had it backwards and none of the LEDs lit.
- Repeating the same block for every LED works, but it doesn't scale.

## Next
Remove the repetition with arrays (and later structs), so adding another LED means adding data, not copying code.