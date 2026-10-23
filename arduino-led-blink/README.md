# arduino-led-blink

The first project: blink the Arduino's built-in LED.

## What it does
The LED on pin 13 turns on for 1 second, then off for 1 second, repeating forever.

## How it works
1. `setup()` runs once and sets pin 13 as an output with `pinMode`.
2. `loop()` runs over and over:
   - `digitalWrite(13, HIGH)` turns the LED on.
   - `delay(1000)` waits 1000 ms.
   - `digitalWrite(13, LOW)` turns the LED off.
   - `delay(1000)` waits another 1000 ms.

## Wiring
No external parts. Pin 13 is connected to the LED built into the Arduino Uno board, so nothing needs to be wired.

Board: Arduino Uno (simulated in Wokwi with PlatformIO in VS Code)

## Result
The built-in LED blinks once per 2-second cycle: 1 second on, 1 second off.

## What I learned
- `pinMode` sets a pin as an input or an output.
- `digitalWrite` sets an output pin `HIGH` or `LOW`.
- `delay` pauses the program, and nothing else can happen while it waits. (This becomes a problem later. See `arduino-millis-blink`.)

## Next
Reading a button with `digitalRead` (see `arduino-button-input`).