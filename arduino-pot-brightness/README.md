# Arduino Potentiometer Brightness

Turning a potentiometer controls the brightness of an LED on pin 3 in real time, using `analogRead` and PWM.

## Hardware

- Arduino Uno (simulated in Wokwi)
- 1 potentiometer
- 1 LED
- 1 × 150 Ω resistor
- Potentiometer wiring: VCC → 5V, GND → GND, SIG (middle pin) → A1
- LED wiring: pin ~3 → resistor → LED anode; LED cathode → GND

## How it works

The potentiometer is a voltage divider. The two outer pins sit across 5V and GND, and the middle pin (the wiper) outputs a voltage between 0 V and 5 V depending on where the knob is. The ratio of the two resistances either side of the wiper sets that voltage.

`analogRead` converts that voltage into a number from 0 to 1023 (the Uno's 10-bit ADC). `analogWrite` takes a number from 0 to 255. The loop scales one range onto the other:

`led_value = potentiometer_value * MAX_BRIGHTNESS / ANALOG_READ_MAX`
The scaled value goes to `analogWrite` on pin 3, so the PWM duty cycle follows the knob.

## Code notes

- **Pin A1** is read with `analogRead`. `pinMode` is not strictly required for `analogRead`, but is kept to make the pin's role clear.
- **Overflow trap.** On the Uno an `int` is 16 bits, so it holds at most 32,767. `1023 * 255` is 260,865, so doing the multiplication in `int` overflows. Converting one operand to `long` first makes the multiplication happen in 32 bits.
- **Where the cast goes.** The cast has to be applied to an operand, before the multiplication. `long(a * b)` converts an answer that has already overflowed.
- **Local variables.** The values are declared inside `loop()` because they are only needed there.

## How to run

1. Open the `arduino-pot-brightness` folder in VS Code (the folder containing `platformio.ini`).
2. Build with PlatformIO.
3. Start the Wokwi simulator (`F1` → "Wokwi: Start Simulator"), then turn the knob.

## What I learned

- **Reading a pin.** I first stored the pin number in my variable instead of calling `analogRead`, the same mistake I made with `digitalRead`.
- **Integer division.** `(value / 1023) * 255` always gives 0, because `value / 1023` is a whole-number division that rounds down. Multiply first.
- **Right output function.** `digitalWrite` only gives on or off. `analogWrite` gives the PWM levels needed for dimming.
- **Integer overflow.** A 16-bit `int` wraps around, so I needed a `long` in the maths, and the cast has to go on an operand.
- **Voltage divider.** A potentiometer's middle pin outputs a voltage set by the ratio of two resistances.
- **The resistor.** The LED still needs its series resistor, which I forgot at first.