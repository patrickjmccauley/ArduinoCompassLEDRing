# Board info
Board used: [ELEGOO Nano Board CH 340/ATmega+328P](https://www.amazon.com/ELEGOO-Arduino-ATmega328P-Without-Compatible/dp/B0713XK923)

1. Set board to `Arduino Nano`
2. Set bootloader to `ATMega328P (Old Bootloader)


# Installation
1. Install arduino IDE
2. Configure board per board info above
3. Install required adafruit libraries and dependencies:
   - [Neopixel library](https://github.com/adafruit/Adafruit_NeoPixel) for LED ring
   - [HMC5883 library](https://github.com/adafruit/Adafruit_HMC5883_Unified) for compass module

# Things to change if desired
```cpp
// Update the number of pixels if ring size is different than 60
#define NUMPIXELS 60

...

void loop() {
  ...
  // update second and third args for center LED feather distance
  // (how many on each side are also same center color) & outer
  // feather distance, respectively
  setAndFeatherPin(truncatedPin, 1, 3);
  ...
}
```

# Wiring
## Compass module
```
# compass <> arduino

VCC <> 3V3
GND <> GND
SCL <> A5
SDA <> A4
```
## LED Ring
```
# Neopixel ring <> arduino
GND <> GND
5V <> 5V
DIN/DOUT <> D6
```