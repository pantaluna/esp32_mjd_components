# SOP: setup the RTC board as an external crystal oscillator 32Khz for the ESP32

## General
The ESP32's internal 150Hz crystal oscillator is not accurate enough for e.g. data loggers which wake up from deep sleep at regular intervals. It deviates +-1 minute every 4 hours!

Luckily the ESP32 also supports an external 32kHz crystal oscillator. 

@important The ZS-042 RTC board and the Chronodot RTC board can be used for this purpose. The black/yellow RTC board cannot be used as such because the 32K output pin of the DS3231 chip is not exposed.

Status Register (0Fh) Bit 3 "Enable 32kHz Output (EN32kHz)": This bit controls the status of the 32kHz pin. When set to logic 1, the 32kHz pin is enabled and outputs a 32.768kHz squarewave signal. When set to logic 0, the 32kHz pin goes to a high-impedance state. THE INITIAL POWER-UP STATE OF THIS BIT IS LOGIC 1, AND A 32.768KHZ SQUARE-WAVE SIGNAL APPEARS AT THE 32KHZ PIN after a power source is applied to the DS3231 (if the oscillator is running).

## Wiring
```
- Connect RTC board pin VCC => microcontroller pin VCC 3.3V
- Connect RTC board pin GND => microcontroller pin GND
- Connect RTC board pin "32K" => microcontroller pin GPIO#33 (ESP32 PIN XTAL32K_XN)
- Connect RTC board pin "32K" => 4.7K pullup resistor => microcontroller pin VCC 3.3V
```

## Setup
- Goto the project directory, for example `cd ~/my_ds3231_clock_using_lib`
- Run `make menuconfig`.
- Select " Component config  ---> ".
- Select " ESP32-specific  --->".
- Select " RTC clock source ()  --->".
- Select "(X) External 32kHz crystal".
- Exit menuconfig.
- Start your project, for example `make flash monitor`

## Notes
- The setup to use the DS3231 as an RTC Real Time Clock via the I2C protocol, as described in the main document, is not required.
- A battery on the RTC board is not required if you only use this board as an external 32Khz crystal oscillator for the ESP32.
