# Embedded Engineering Plan

Self-directed journey into embedded systems development, following an 11-month learning plan — starting from Arduino fundamentals and progressing toward deeper embedded topics (timers, UART, I2C, SPI, state machines, FreeRTOS).

Each folder is a standalone project, simulated using [Wokwi](https://wokwi.com) and built with PlatformIO.

## Projects
- `arduino-led-blink/` — Basic digital output: blinking the built-in LED using `pinMode`/`digitalWrite`/`delay`.
- `arduino-button-input/` — Digital input: reading a push button with `digitalRead` and `INPUT_PULLUP` to control an LED.
- `arduino-pwm-fade/` — Analog output: smoothly fades an LED on pin 3 from off to full brightness and back over and over with `analogWrite` using PWM 
- `arduino-pot-brightness/` — Analog input: reading a potentiometer with `analogRead` and mapping its 0–1023 range onto PWM `analogWrite` to control LED brightness.