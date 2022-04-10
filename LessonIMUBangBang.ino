/*
  Using the BNO080 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to output the parts of the calibrated gyro.

  It takes about 1ms at 400kHz I2C to read a record from the sensor, but we are polling the sensor continually
  between updates from the sensor. Use the interrupt pin on the BNO080 breakout to avoid polling.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 115200 baud to serial monitor.
*/

#include <Wire.h>

#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080
BNO080 myIMU;

#define BLUELED 13
#define YELLOWLED 14
#define LIMIT 0.3

#define ARRAYSIZE 4

float signals[ARRAYSIZE]={0.0};

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO080 Read Example");

  Wire.begin();


  pinMode(BLUELED, OUTPUT); 
  pinMode(YELLOWLED, OUTPUT); 
  myIMU.begin();
  
  Wire.setClock(400000); //Increase I2C data rate to 400kHz
  myIMU.enableGyro(50); //Send data update every 50ms

  Serial.println(F("Gyro enabled"));
  Serial.println(F("Output in form x, y, z, in radians per second"));
}

float smooth(float tmp){
  int count;
  for(count=0;count<(ARRAYSIZE-1);count++){
    signals[count]=signals[count+1];
  }
   signals[count]=tmp;

   float favg=0.0;

  for(count=0;count<ARRAYSIZE;count++){
    favg=signals[count];
  }
  favg/=ARRAYSIZE;
  return favg;     
}



float activate(float fval){   
  fval=smooth(fval);

  if(fval>LIMIT)
    digitalWrite(BLUELED, HIGH);
  else
    digitalWrite(BLUELED, LOW);

  if(fval<-LIMIT)
    digitalWrite(YELLOWLED, HIGH);
  else
    digitalWrite(YELLOWLED, LOW);
}

void loop()
{
  //Look for reports from the IMU
  if (myIMU.dataAvailable() == true)
  {
    float x = myIMU.getGyroX();
    float y = myIMU.getGyroY();
    float z = myIMU.getGyroZ();

    
    Serial.print(x, 2);
    Serial.print(F(","));
    Serial.print(y, 2);
    Serial.print(F(","));
    Serial.print(z, 2);
    Serial.print(F(","));

    Serial.println();

    activate(x);      
  }
}
