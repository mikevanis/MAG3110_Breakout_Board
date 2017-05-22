/*  *********************************************
 *  SparkFun_MAG3110_Manual_Calibration
 *  Triple Axis Magnetometer Breakout - MAG3110
 *  Hook Up Guide Example
 *
 *  Utilizing Sparkfun's MAG3110 Library
 *  
 *  Calibrates the magnetometer by finding the x and y min and max.
 *  Does not use the library's internal calibration routine. 
 *
 *  Mike Vanis
 *  Created: Jan 2017
 *  Updated: n/a
 *
 *  Development Environment Specifics:
 *  Arduino 1.6.7
 *
 *  Hardware Specifications:
 *  SparkFun MAG3110
 *  Bi-directional Logic Level Converter
 *  Arduino Uno 
 *
 *  *********************************************/

#include <SparkFun_MAG3110.h>
#include "Wire.h"

boolean calibrated = false;

MAG3110 mag = MAG3110(); //Instantiate MAG3110

void setup() {
  Serial.begin(9600);

  mag.initialize(); //Initializes the mag sensor
  mag.start();      //Puts the sensor in active mode

  int16_t xMax, xMin, yMax, yMin;
  int16_t prev_xMax, prev_xMin, prev_yMax, prev_yMin;
  int sameValueCount;
  boolean firstValue = true;
  while (!calibrated) {
    int16_t x, y, z;
    mag.readMag(&x, &y, &z);

    if (firstValue) {
      xMax = x;
      xMin = x;
      yMax = y;
      yMin = y;
      firstValue = false;
    }
    else {
      if (x > xMax) xMax = x;
      else if (x < xMin) xMin = x;

      if (y > yMax) yMax = y;
      else if (y < yMin) yMin = y;

      if (xMax == prev_xMax && xMin == prev_xMin && yMax == prev_yMax && yMin == prev_yMin) {
        sameValueCount++;
        Serial.println("Same value");
      }
      else {
        sameValueCount--;
        if (sameValueCount < 0) sameValueCount = 0;
      }

      if (sameValueCount > 500) {
        calibrated = true;
        int16_t xOffset = (xMin + xMax) / 2;
        int16_t yOffset = (yMin + yMax) / 2;
        mag.setOffset(MAG3110_X_AXIS, xOffset);
        mag.setOffset(MAG3110_Y_AXIS, yOffset);
      }

      Serial.print("xMax: ");
      Serial.print(xMax);
      Serial.print("\txMin: ");
      Serial.print(xMin);
      Serial.print("\tyMax: ");
      Serial.print(yMax);
      Serial.print("\tyMin: ");
      Serial.println(yMin);

      prev_xMax = xMax;
      prev_xMin = xMin;
      prev_yMax = yMax;
      prev_yMin = yMin;
    }
  }
}

void loop() {

  int16_t x, y, z;
  //Only read data when it's ready
  if (mag.dataReady()) {
    //Read the data
    mag.readMag(&x, &y, &z);

    float heading = (atan2(y, x) * 180) / 3.14159;

    if (heading < 0) heading = 360 + heading;

    Serial.print("X: ");
    Serial.print(x);
    Serial.print("\tY: ");
    Serial.print(y);
    Serial.print("\tH: ");
    Serial.println(heading);

    Serial.println("--------");
  }
}

