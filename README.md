# Lightmeter
A Lightmeter/Flashmeter for photographers, based on Arduino.

Fork of `lightmeter` by @vpominchuk

Components:
- Arduino UNO R3
- BH1750FVI digital light sensor
- SSD1306 OLED 0.96 128x64 I2C
- Buttons
- 50x70mm PCB
- AAA battery holder

The lightmeter based on Arduino as a main controller and BH1750 as a metering cell. Information is displayed on SSD1306 OLED display. The device is powered by 2 AAA batteries.

Functions list:

* Ambient light metering
* Flash light metering
* ND filter correction
* Aperture priority
* Shutter speed priority
* ISO range 8 - 4,000,000
* Aperture range f/1.0 - f/3251
* Shutter speed range 1/10000 - 133 sec
* ND Filter range ND2 - ND8192
* Displaying amount of light in lux.
* Displaying exposure value (EV)
* Recalculating exposure pair while one of the parameter changing
* Battery information
* Power 4xAAA batteries
