# ESP-IDF MJD Adafruit HUZZAH32 component
This component has been developed to expose specific functionality for the Adafruit HUZZAH32 development board (ESP32).

This component is based on the ESP-IDF V3 framework for the ESP32 hardware from Espressif.



**This component contains the following logic:**

- To read the actual battery voltage level.
- To configure the ESP32 so that the VREF (Voltage Reference) of the ESP32 can be read on GPIO#26 using e.g. a multimeter. The VREF varies slightly across dev boards. The exact value is crucial for calibrating the ADC module accurately. This measurement is only required once to obtain the exact value. This value is used afterwards to get accurate readings of the battery voltage level.

Check the header file for more documentation.



## Example ESP-IDF project
my_huzzah32_battery_voltage_using_lib

## Shop Products
Adafruit HUZZAH32

## Data Sheets
https://learn.adafruit.com/adafruit-huzzah32-esp32-feather



## Wiring Instructions
Connect a LiFePO4 3.2V battery to the "3.3V" pin, or connect a Lion/LiPo 3.7V battery to the battery connector (ensure you have the polarity correct).

Goto the directory ../../development_boards/ for images with the GPIO PIN layout for some development boards.



## FAQ
- The Adafruit HUZZAH32 is the most popular made-in-the-USA ESP32 development board. They are more expensive than the Chinese ESP32 development boards but they are worth the money (Adafruit is known for good technical documentation).
- The HUZZAH32 will trickle-charge the battery when the USB cable is attached and the battery is connected to the LiPO connector.



## Issues


## Reference: the ESP32 MJD Starter Kit SDK

Do you also want to create innovative IoT projects that use the ESP32 chip, or ESP32-based modules, of the popular company Espressif? Well, I did and still do. And I hope you do too.

The objective of this well documented Starter Kit is to accelerate the development of your IoT projects for ESP32 hardware using the ESP-IDF framework from Espressif and get inspired what kind of apps you can build for ESP32 using various hardware modules.

Go to https://github.com/pantaluna/esp32-mjd-starter-kit

