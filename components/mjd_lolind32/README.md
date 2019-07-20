# ESP-IDF MJD LOLIN D32 component
This component "mjd_lolind32" has been developed to expose specific functionality for the WEMOS LOLIN D32 development board (ESP32). This component is developed for the ESP-IDF framework for the ESP32 hardware from Espressif.

The features of this component:

* To obtain with a simple function call the actual battery voltage of the Lion battery that is hooked up to the JST-PH2 connector.
* To verify what method will be used by the ESP-IDF ADC component in relation to the calibration of the ADC. The ADC peripheral of the ESP32 is used for example to read the battery voltage.
* To verify the actual VREF Voltage Reference of the ESP32. This can be done by routing the actual voltage reference value to an analog GPIO# using the ADC1 or the ADC2 peripheral. And then use a multimeter to verify the VREF voltage. The value will be around 1100mV.



Check the header file for more documentation.



## Background about the calibration the ADC of the ESP32

https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/adc.html#adc-calibration

> The calibration of the ADC can be achieved by various means. 1) eFuse Vref. This value is measured and burned into eFuse BLOCK0 during factory calibration; 2) Two Point; 3) Default Vref specified by the user/app.



Three calibration characteristic mechanisms:

* eFuse "TP Two Point" BLOCK3: characterization based on Two Point values.

* eFuse "VREF Voltage Reference" BLOCK0: characterization based on the reference voltage. Example dev boards: LOLIN D32.

* "Default Vref": the ADC reference voltage is provided by the user as a parameter during characterization. Typical value is 1100mV. Example dev boards: Adafruit HUZZAH32.

If "Two Point" or "eFuse Vref" values are unavailable, then the "Default Vref" mechanism will be used.

   

**For the LOLIN D32 the "eFuse VREF Voltage Reference" method is used to represent the ADC reference voltage. It is not required to specify a value for the ADC calibration characteristics to read the voltage via ADC (in fact the manually specified value would be ignored as the eFuse value will be used).**



## Example ESP-IDF project
my_lolind32_battery_voltage_using_lib



## Shop Products
LOLIN D32 https://wiki.wemos.cc/products:d32:d32



## Data Sheets
https://wiki.wemos.cc/products:d32:d32



## Wiring Instructions
Connect a Lion 3.7V battery to the battery connector (ensure you have the polarity correct).

Goto the directory ../../development_boards/ for more information about the development boards.



## Issues

None.



## Reference: the ESP32 MJD Starter Kit SDK

Do you also want to create innovative IoT projects that use the ESP32 chip, or ESP32-based modules, of the popular company Espressif? Well, I did and still do. And I hope you do too.

The objective of this well documented Starter Kit is to accelerate the development of your IoT projects for ESP32 hardware using the ESP-IDF framework from Espressif and get inspired what kind of apps you can build for ESP32 using various hardware modules.

Go to https://github.com/pantaluna/esp32-mjd-starter-kit

