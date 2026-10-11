# Arduino PWM Fade

An LED on pin 3 fades smoothly from off to full brightness and back, over and over, using PWM.

## Hardware

- Arduino Uno (simulated in Wokwi)
- 1 LED
- 1 × 150 Ω resistor
- Wiring: pin ~3 → resistor → LED anode; LED cathode → GND

## How it works

`analogWrite(pin, value)` takes a value from 0 to 255 and switches the pin on and off very fast (PWM, pulse width modulation). The value sets the duty cycle, which is the share of each cycle the pin spends HIGH. A value of 128 is about 50%, so the pin is HIGH half the time and LOW half the time.

The LED flickers too fast to see, so it looks dimmer instead of blinking. The brightness follows the average voltage. At 25% duty cycle the pin is HIGH for one quarter of each cycle and LOW for three quarters, which averages about 1.25 V out of 5 V.

The loop steps the brightness up from `MIN_BRIGHTNESS` to `MAX_BRIGHTNESS` in increments of `FADE_STEP`. Then it steps back down, skipping the peak and bottom values so they aren't held twice.

## Code notes

- **Pin 3, not pin 13.** Only pins marked `~` (3, 5, 6, 9, 10, 11) support PWM. Pin 13 does not.
- **Named constants.** `FADE_STEP`, `STEP_DELAY_MS`, `MAX_BRIGHTNESS` and `MIN_BRIGHTNESS` replace bare numbers, so each value has a meaning and can be changed in one place.
- **Step-size rule.** `FADE_STEP` must divide `MAX_BRIGHTNESS` evenly (255 = 3 × 5 × 17). A step like 20 skips past 255 and the fade never reaches the top.

## How to run

1. Open the `arduino-pwm-fade` folder in VS Code (the folder containing `platformio.ini`).
2. Build with PlatformIO (`Build` in the PlatformIO toolbar).
3. Start the Wokwi simulator (`F1` → "Wokwi: Start Simulator").

## What I learned

- **The second-loop bug.** My fade-down loop never ran because it started at 260 and checked `brightness <= 0`, which is false from the start. It needed `>=` and a starting value of 255.
- **Duty cycle and average voltage.** PWM doesn't lower the voltage. It changes how long the pin stays HIGH, and the LED responds to the average.
- **The resistor is still needed.** PWM controls brightness, but the resistor limits current whenever the pin is HIGH.
- **Named constants** make the code readable and safer to change.
- **The step-size trap.** A step that doesn't divide 255 evenly overshoots the maximum.