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

void setAndFeatherPin(int centerPin, int centerPinAddlPixelWidthEachSide, int featherDistanceEachSide) {
  // change RGB values here
  pixels.setPixelColor(centerPin, pixels.Color(0, 255, 0));

  for (int i = 1; i < centerPinAddlPixelWidthEachSide + 1; i++) {
    ...
    // change the RGB values here for inner feather
    pixels.setPixelColor(leftTargetPin, pixels.Color(0, 255, 0));
    pixels.setPixelColor(rightTargetPin, pixels.Color(0, 255, 0));
  }

  for (int i = 1; i < featherDistanceEachSide + 1; i++) {
    ...
    // change RGB values here for the outer feather
    pixels.setPixelColor(leftTargetPin, pixels.Color(255, 234, 0));
    pixels.setPixelColor(rightTargetPin, pixels.Color(255, 234, 0));
  }
}


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

# Troubleshooting
If you have issues trying to load to the board, do the following:
1. Unplug the board and plug back in
2. Try a different USB cable
3. Open the arduino IDE and go to Tools > Get Info. If you get some info, you at least have connectivity
4. run `sudo chown yourname:yourname /dev/ttyUSB0`, replacing `yourname` and the filepath with the appropriate username + tty device port
