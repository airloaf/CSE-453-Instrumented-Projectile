# CSE-453-Instrumented-Projectile

## Introduction

This project is for our Senior design course CSE 453, Hardware Software Integration. Our team was tasked with creating an instrumented projectile capable of taking and storing measurements of its trajectory during launch from an air cannon. The projectile must be capable of withstanding multiple launches. The cannon and subsequently the projectile is made to be used as a demonstration of projectile motion in a school setting.

The Instrumented projectile will house a microcontroller circuit that is outfitted with the necessary sensors to take various measurements, and will be powered by a small rechargeable lithium ion polymer battery. Once the projectile is activated and launched, it will store its real time measurements to the on-board SD card. The user will then be able to review the trajectory measurements after retrieving the projectile.

## Parts List

### Core Parts

-[Arduino Trinket M0](https://www.adafruit.com/product/3500)

-[Adafruit MPU-6050 6-DoF Accel and Gyro Sensor](https://www.adafruit.com/product/3886)

-[Adafruit LiIon/LiPoly Backpack Add-On for Pro Trinket/ItsyBitsy](https://www.adafruit.com/product/2124)

-[Micro SD Card Writer](https://www.adafruit.com/product/254)

### Batteries

-[Lithium Ion Polymer Battery - 3.7v 1200mAh](https://www.adafruit.com/product/258)

-[Lithium Ion Polymer Battery - 3.7v 500mAh](https://www.adafruit.com/product/1578)

-[Lithium Ion Polymer Battery - 3.7v 150mAh](https://www.adafruit.com/product/1317)

### Other Parts

-[Arduino Stackable Header - 6 Pin](https://www.sparkfun.com/products/9280)

-[Foam Balls](https://www.amazon.com/Tourna-Tennis-Practice-Youth-Training/dp/B0076ZITY6/ref=sr_1_51?dchild=1&keywords=3%22+ball+made+of+soft+foam&qid=1618164029&sr=8-51)

-[Piezo Buzzer](https://www.adafruit.com/product/160)

-[Tennis Balls](https://www.amazon.com/Wilson-Sporting-Goods-Prime-Tennis/dp/B07CFH181X/ref=sr_1_2?dchild=1&keywords=tennis&qid=1616775832&sr=8-2&th=1&psc=1)

-[USB SD Card Reader](https://www.amazon.com/Adapter-Standard-Connector-Smartphones-Function/dp/B01BXSKPES)

## Installing Source Code to Trinket M0

1. Install Circuit Python: ([Trinket M0 Circuit Python Installation](https://learn.adafruit.com/adafruit-trinket-m0-circuitpython-arduino/circuitpython))

2. Copy the contents of ```src/``` into the root directory of the Trinket M0

3. Copy ```libs/Circuit Python``` into the lib folder of the Trinket M0


## Directory Structure

```
docs/               Documentation for assembling the projectile
HardwareTests/      Tests for hardware components in isolation
libs/               Libraries either for the microcontroller code or for fritzing circuits
models/             3D models for the projectile
src/                Circuit Python Source Code
```
