# arduino-external-interrupt

A button wired to an interrupt pin toggles an LED, with no button check in `loop()`. The hardware calls a function when the button is pressed. Two counters show how many times that function ran and how many of those runs counted as real presses.

## What it does
- Pressing the button toggles the LED on pin 6.
- After each press, the Serial monitor prints how many times the interrupt fired and how many presses passed the debounce check.

## How it works
`attachInterrupt()` on pin 2 calls `on_button_press()` on every `FALLING` edge, which is the moment the pin goes from HIGH to LOW. The button uses `INPUT_PULLUP`, so pressing it pulls the pin to GND.

1. **The function stays short.** It adds one to `isr_counter`, reads `millis()` once, and compares it with the time of the last accepted press. If at least 50 ms has passed, it toggles `state`, saves the time and adds one to `accepted_presses`. Otherwise it does nothing.
2. **`loop()` does the work.** It writes `state` to the LED and prints the counters only when `isr_counter` has changed. Each counter is copied once into a local variable first, so the printed numbers stay consistent.
3. **Counters as `byte`.** On the Uno's 8-bit CPU a `byte` is read in one instruction, so `loop()` can never read half of a value the interrupt is changing. A 2-byte `int` could be read in two steps, with the interrupt firing in between.

## Wiring
| Part | Pin |
|---|---|
| Button, one side | pin 2 (interrupt pin) |
| Button, other side | GND |
| LED anode | pin 6, through a 220 Ω resistor |
| LED cathode | GND |

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code). The button uses the internal pull-up resistor.

## Result
One tap of the button fired the interrupt 146 times, and 2 of those passed the 50 ms debounce check. The first burst of about 75 edges was the press bouncing, and the debounce rejected it correctly. The second burst was the release bouncing. Its first edge came after the 50 ms lockout had ended, so it counted as a new press.

## Known limit
A press held for longer than 50 ms followed by a bouncing release can toggle the LED twice. The program only listens for the falling edge, so it never sees the rising edge of the release and the timer is not restarted when the button is let go. A fix I have not built yet: trigger on `CHANGE`, restart the lockout timer on every edge, and toggle only when the pin reads LOW. A small capacitor across the button on real hardware would also smooth it out.

## What I learned
- On the Uno, `attachInterrupt()` works only on pins 2 and 3, because those two are wired to the chip's external interrupt hardware.
- A variable shared between an ISR and `loop()` must be `volatile`. Without it the compiler may keep a copy in a register and never notice the ISR changed it.
- `delay()` does not work inside an ISR, because it relies on the timer interrupt that updates `millis()`, and that interrupt is blocked while the ISR runs. `Serial.print()` can block when its buffer is full. Reading `millis()` once is fine.
- The ISR should do the minimum: record that something happened and leave. The real work belongs in `loop()`.
- A bounce is not one extra edge, it is dozens: one tap gave 146 interrupts. The counters made that visible, and a bounce on release can break a debounce that only handles the press.
- The debounce from my earlier button project needed moving into the ISR, and the same idea (remember the last accepted time, ignore anything sooner) carries over.
- Printing every pass of `loop()` flooded the monitor. Printing only when a value changes fixed it.

## Next
Hardware timers: letting the chip's timer call a function at a fixed rate, with no `millis()` check.