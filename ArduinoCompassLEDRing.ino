#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        6
#define NUMPIXELS 60
#define addr 0x1E //I2C Address for The HMC5883
#define DELAYVAL 250

// logging
#define SHOULD_LOG_COMPASS false
#define SHOULD_LOG_LED_RING true

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  Serial.begin(9600);
  Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  // ===============================
  // Initialize compass module
  // ===============================
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");

  
  // ===============================
  // Initialize LED ring module
  // ===============================
  pixels.begin();
  pixels.setBrightness(50);
}

float getRatioOfDegrees() {
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);

  if (SHOULD_LOG_COMPASS) {
    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
  }
 
  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  if (SHOULD_LOG_COMPASS) {
    Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  }
  float ratio = headingDegrees / 360.0;
  return ratio;
}

void setAndFeatherPin(int centerPin, int centerPinAddlPixelWidthEachSide, int featherDistanceEachSide) {
  pixels.setPixelColor(centerPin, pixels.Color(0, 255, 0));

  for (int i = 1; i < centerPinAddlPixelWidthEachSide + 1; i++) {
    int leftTargetPin = (centerPin + NUMPIXELS - i) % NUMPIXELS;
    int rightTargetPin = (centerPin + i) % NUMPIXELS;
    pixels.setPixelColor(leftTargetPin, pixels.Color(0, 255, 0));
    pixels.setPixelColor(rightTargetPin, pixels.Color(0, 255, 0));
  }
  
  for (int i = 1; i < featherDistanceEachSide + 1; i++) {
    int leftTargetPin = (centerPin + NUMPIXELS - i - centerPinAddlPixelWidthEachSide) % NUMPIXELS;
    int rightTargetPin = (centerPin + i + centerPinAddlPixelWidthEachSide) % NUMPIXELS;
    pixels.setPixelColor(leftTargetPin, pixels.Color(255, 234, 0));
    pixels.setPixelColor(rightTargetPin, pixels.Color(255, 234, 0));
  }
}

void loop() {
  float ratio = getRatioOfDegrees();
  int truncatedPin = ((((int) (ratio * NUMPIXELS)) + NUMPIXELS) - 1) % NUMPIXELS;
  if (SHOULD_LOG_LED_RING) {
    Serial.print("Translated to pin: "); Serial.println(truncatedPin); 
  }
  pixels.clear();
  setAndFeatherPin(truncatedPin, 1, 3);
  pixels.show();
  delay(DELAYVAL);
}
