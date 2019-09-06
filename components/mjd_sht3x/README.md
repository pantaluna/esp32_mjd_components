# ESP-IDF MJD SHT3x Relative Humidity and Temperature Sensor

This is component for the ESP-IDF software framework of the ESP32 hardware from Espressif.

This component is developed for **the Sensirion SHT3x Relative Humidity and Temperature Sensor product range** and the **Sensirion SHT85 product**. These are a few products of that range:

- SHT30-DIS-B
- SHT31-DIS-B
- SHT31-DIS-F
- SHT31-DIS-P
- SHT35-DIS-B
- SHT35-DIS-F
- SHT85

The ESP-IDF component configures the SHT3x device as a I2C slave device and returns **the temperature in degrees Celsius and Fahrenheit, the relative humidity in % and the dew point in degrees Celsius and Fahrenheit**.

More information:
- Sensirion's Digital Humidity Sensor SHT3x (RH/T) 
  <https://www.sensirion.com/en/environmental-sensors/humidity-sensors/digital-humidity-sensors-for-various-applications/>



## Example ESP-IDF project(s)

Go to the examples and learn how the component is used.

```esp32_sht3x_sensor_using_lib``` This project demonstrates the component mjd_sht3x.




## Shop Product

[ Goto the _doc folder for photo's.]

- Adafruit Sensirion SHT31-D - Temperature & Humidity Sensor.
- SHT31 Temperature & Humidity Sensor Module Breakout Weather (various vendors).
- Sensirion SHT85 Humidity & Temperature Sensor ±1.5% RH ±0.1°C I2C (a high quality mini-board made by Sensirion).



## Wiring Instructions

### General

The specific wiring instructions are documented in the  ESP-IDF example project(s).



## Pullup Pulldown Resistors

The breakout boards mentioned above contain **a 10K pullup resistor** for each of these pins: **SCL, SDA**.

The breakout boards mentioned above contain a **10K pulldown resistor** for this pin: **AD** (address).



### PIN layout of the breakout board : Sensirion SHT31 Temperature & Humidity Sensor Module Breakout Weather

[ Goto the _doc folder for photo's.]

```
PIN#      PIN NAME	 Description
--------  ---------- -----------
Left 1    VIN        Power supply (3.3V for the ESP32)
Left 2    GND        Ground
Left 3    SCL        I2C Serial Clock
Left 4    SAA/SDA    I2C Serial Data
Right 1   AL         Alert pin
Right 2   AD         I2C Address pin
```

Note: wiring up the pins AL and AD is optional.



### Wiring for the I2C protocol

- Connect device pin "VIN" to the MCU pin VCC (3.3V).
- Connect device pin "GND" to the MCU pin GND.
- Connect device pin "SCL" to the MCU pin SCL. I use GPIO#21 on the HUZZAH32 dev board.
- Connect device pin "SAA/SDA" to the MCU pin SDA. I use GPIO#17 on the HUZZAH32 dev board).



## Device I2C protocol

- The device acts as a slave.
- The IC supports various I2C clock speeds from 10 KHz to 1 Mhz. **The maximum I2C speed of an ESP32's I2C Master is 1 MHz.**
- @tip Using very low speeds such as 10 Khz when using long wires (>25cm) is also supported.



### Device I2C Slave Address

- **Default I2C Address: 0x44.**
- Use the project ```esp32_i2c_scanner``` to detect the actual I2C Address.
- Wire up a 4.7K pullup resistor to the pin "AD" to use the I2C Address 0x45.



### I2C and wire length

- It is important to have stable I2C connections.
- Use **good quality breadboards**. Many have bad contacts, especially the ones that have been used for a month in the lab.
- Use **short, good quality Dupont cables**. In my experience a stock Dupont cable of 30cm is too long when using these modules on a breadboard (especially for the SCL and SDA connections). A 5cm Dupont cable always works.
- It is better to solder everything together on **a PCB** as soon as possible and use quality AWG22 wires and connectors.
- Guidelines "Topic: I2C flakiness:  best strategy to identify and fix?" https://forum.arduino.cc/index.php?topic=509323.0



## Data Sheet
[Go to the _doc directory for more documents and images.]

<https://www.sensirion.com/en/environmental-sensors/humidity-sensors/digital-humidity-sensors-for-various-applications/>




## IC/Module/ESP-IDF Component FAQ
- Operating voltage 2.15V - 5.5V.
- The market offers several breakout modules which are very handy.
- Check **the data sheet ** for detailed information.
- Check **the documented example projects and the sources of this component** for practical information.
- The following properties can be configured in the component: I2C slave address, I2C timeout, I2C port (I2C_NUM_0 or I2C_NUM_1), GPIO# for SCL, GPIO# for SDA,  SHT3x Repeatability. The defaults can be changed in the component configuration.
- The component returns the following results of each measurement:
  - Temperature in degrees Celsius and in Fahrenheit.
  - Relative Humidity %.
  - Dew Point in degrees Celsius and Fahrenheit (<https://en.wikipedia.org/wiki/Dew_point>). It is a complicated formula so it is handy that the component calculates that for you :)
- The default for the **SHT3x Repeatability** setting in the component is **High**. The property influences the measurement duration, noise levels and the overall energy consumption of the sensor. The measurement duration for each measurement is: High=15 ms, Medium=6 ms, Low=4 ms.
- The component implements the **Single Shot Mode**. Note that you can also read measurements in this mode at relatively high speed (6 - 25Hz depending on the Repeatability setting).
- The device's **Single Shot Mode with I2C Clock Stretch enabled** is not implemented in the component because I2C Clock Stretching is not supported properly in the ESP32 I2C Driver / Hardware.
- The component can be used to **read/write all documented properties in the device** registers. Check the source ```mjd_sht3x_defs.h``` for more information.
- The **Periodic Data Acquisition Modes** are not implemented in this component. This component was made for typical IoT sensor nodes where that feature is not required; and not desired because of its high power consumption. You can still implement periodic measurements in your project by executing periodic Single Shot Mode measurements at relatively high speed (6 - 25Hz depending on the Repeatability setting).
- The use of **the ALERT pin** is not implemented. It is typically used in a hardware setup without a microcontroller.



## Issues

- The SHT3x device must be Soft Reset after the ESP32 I2C Driver has been activated else subsequent measurements will always fail. That problem is handled transparently in this component.


- The SHT3x device implements a Last Command Status information in the Status Register. It is supposedly to be used to check if the last command was processed properly by the device (the CRC and the parameters were correct). My experience is that this status is never updated so it is not used in this component.




## Reference: the ESP32 MJD Starter Kit SDK

Do you also want to create innovative IoT projects that use the ESP32 chip, or ESP32-based modules, of the popular company Espressif? Well, I did and still do. And I hope you do too.

The objective of this well documented Starter Kit is to accelerate the development of your IoT projects for ESP32 hardware using the ESP-IDF framework from Espressif and get inspired what kind of apps you can build for ESP32 using various hardware modules.

Go to https://github.com/pantaluna/esp32-mjd-starter-kit

