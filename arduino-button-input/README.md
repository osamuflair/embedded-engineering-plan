# Arduino Button Input

Reads a push button and lights the built-in LED only while the button is held down.

## What it does
Holding the button turns the LED on. Releasing it turns the LED off.

## Hardware
- Push button: one terminal on digital pin 2, the other on GND
- LED: the built-in LED on pin 13 (no extra wiring)

No external resistor is needed because the pin uses the Arduino's internal pull-up.

## How it works
The pin is set to `INPUT_PULLUP`, which connects it to 5V through an internal resistor. With the button released, the pin reads HIGH. Pressing the button connects the pin to GND, so it reads LOW. That means the logic is inverted: pressed is LOW, released is HIGH. Without the pull-up, a released button would leave the pin "floating" and the reading would be unpredictable.

`loop()` calls `digitalRead` on every pass. If the pin is LOW, the LED turns on, otherwise it turns off.

## How to run
1. Open this folder in VS Code (the one containing `platformio.ini`).
2. Build with PlatformIO (checkmark in the status bar).
3. Run "Wokwi: Start Simulator" and click the button in the simulation.

## What I learned / problems hit
- C++ needs types for variables (`const int`), unlike Python.
- C++ is case-sensitive: `digitalread` is not `digitalRead`.
- A pin number is just a number. You have to call `digitalRead` to get its state.
- Config filenames must be exact. A misspelled `wokwi.toml` meant the simulator couldn't find its config.
- PlatformIO and Wokwi only work when the folder containing their config files is the one open in VS Code.