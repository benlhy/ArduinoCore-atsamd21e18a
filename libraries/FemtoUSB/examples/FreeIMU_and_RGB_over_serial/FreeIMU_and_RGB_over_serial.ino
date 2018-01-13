/**
 * FreeIMU library serial communication protocol
 * 
 * Note, we use MARG 4 in FreeIMU.h
*/
#include <Wire.h>
#include <SPI.h>

#define Serial SERIAL_PORT_USBVIRTUAL

//These are optional depending on your IMU configuration
//#include <ADXL345.h>
//#include <HMC58X3.h>
//#include <LSM303.h>
//#include <LPS.h> 
//#include <ITG3200.h> //note LPS library must come before ITG lib
//#include <bma180.h>
//#include <MS561101BA.h> //Comment out for APM 2.5
//#include <BMP085.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
//#include <AK8975.h>
#include <AK8963.h>
//#include <L3G.h>
//#include <SFE_LSM9DS0.h>
//#include <BaroSensor.h>
#include <AP_Baro_MS5611.h>  //Uncomment for APM2.5


//These are mandatory
#include <AP_Math_freeimu.h>
#include <Butter.h>    // Butterworth filter
#include <iCompass.h>
#include <MovingAvarageFilter.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
//#include "DCM.h"
#include "FilteringScheme.h"
#include "RunningAverage.h"
#include "FreeIMU.h"

//Intel Edison, Arduino 101, Arduino Due, Arduino Zero: no eeprom 
#if defined(__SAMD21G18A__) || defined(_VARIANT_ATSAMR21E18A_) || defined(__SAM3X8E__) || defined(__ARDUINO_ARC__) || defined(__SAMD21G18A__)
  #define HAS_EEPPROM 0
#else
  #include <EEPROM.h>
  #define HAS_EEPPROM 1
#endif

int raw_values[11];
//char str[512];
float ypr[3]; // yaw pitch roll
float val[9];

// Set the FreeIMU object
FreeIMU my3IMU = FreeIMU();

int PIN_R = 4;
int PIN_G = 3;
int PIN_B = 10; // Also the SCK pin!

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  delay(5);
  my3IMU.init(); // the parameter enable or disable fast mode
  delay(5);

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  
}

void loop() { 
  
  my3IMU.getYawPitchRoll(ypr);
  Serial.print("Yaw: ");
  Serial.print(ypr[0]);
  Serial.print(" Pitch: ");
  Serial.print(ypr[1]);
  Serial.print(" Roll: ");
  Serial.print(ypr[2]);
  //Serial.println("");


  // Set RGB colors according to YPR
  int red = 255 - (( (ypr[0] + 180.0f) / 180) * 255);
  int green = 255 - ((abs (ypr[1] + 53.8) / 90) * 255);
  int blue = 255 - ((ypr[2] + 70 + 31.96) / 140) * 255;

  Serial.print(" red: ");
  Serial.print(red);
  Serial.print(" green: ");
  Serial.print(green);
  Serial.print(" blue: ");
  Serial.print(blue);
  Serial.println("");
  
  analogWrite(PIN_R, red );
  analogWrite(PIN_G, green );
  analogWrite(PIN_B, blue );
  
  delay(1);
}