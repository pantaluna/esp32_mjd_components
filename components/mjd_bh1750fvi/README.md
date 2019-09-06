# ESP-IDF BH1750FVI light intensity sensor component
This is component based on ESP-IDF for the ESP32 hardware from Espressif.



## Example ESP-IDF project
`esp32_bh1750fvi_lightsensor_using_lib` This project demonstrates the basics of using the MJD component "mjd_bh1750fvi" for the BG1750FVI light sensor.



## Shop Product
GY-032 BH1750FVI Digital Light Intensity Sunlight Luminescence Sensor Module For AVR Arduino 3V-5V



## Wiring Instructions
### Sensor PIN layout

```
1   VCC
2   GND
3   SCL
4   SDA
5   ADDR
```

### Sensor for the I2C protocol
- Connect device pin 1 VCC to the MCU pin VCC 3V.
- Connect device pin 2 GND to the MCU pin GND.
- Connect device pin 3 SCLK to the MCU GPIO#21 ((Huzzah32 #21 bottomleft).
- Connect device pin 4 SDA to the MCU GPIO#17  ((Huzzah32 #17 bottomleft-1).



## Data Sheet
[Go to the _doc directory for more documents and images.]

http://www.mouser.com/ds/2/348/bh1750fvi-e-186247.pdf

https://www.rohm.com/products/sensors-mems/ambient-light-sensor-ics



## Sensor FAQ
- OK 3.3V
- Metric unit: LUX. Darkness is typically less than 10 lux.
- Chip BH1750FVI from the vendor ROHM.
- The driver uses the default sensitivity of 1.0.



## Sensor & I2C protocol
- I2C device slave address: 0x23.
- It is possible to change the I2C slave-address with pin 5 ADDR:
  - Low  (<= 0.3V) = 0x23 0b0100011 ***(default)***
  - High (>= 0.7V) = 0x5C 0b1011100
- Maximum I2C SCL Clock Frequency: 400 kHz.



## Reference: the ESP32 MJD Starter Kit SDK

Do you also want to create innovative IoT projects that use the ESP32 chip, or ESP32-based modules, of the popular company Espressif? Well, I did and still do. And I hope you do too.

The objective of this well documented Starter Kit is to accelerate the development of your IoT projects for ESP32 hardware using the ESP-IDF framework from Espressif and get inspired what kind of apps you can build for ESP32 using various hardware modules.

Go to https://github.com/pantaluna/esp32-mjd-starter-kit

