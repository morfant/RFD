/* MPU9250 Basic Example Code
  by: Kris Winer
  date: April 1, 2014
  license: Beerware - Use this code however you'd like. If you
  find it useful you can buy me a beer some time.
  Modified by Brent Wilkins July 19, 2016

  Demonstrate basic MPU-9250 functionality including parameterizing the register
  addresses, initializing the sensor, getting properly scaled accelerometer,
  gyroscope, and magnetometer data out. Added display functions to allow display
  to on breadboard monitor. Addition of 9 DoF sensor fusion using open source
  Madgwick and Mahony filter algorithms. Sketch runs on the 3.3 V 8 MHz Pro Mini
  and the Teensy 3.1.

  SDA and SCL should have external pull-up resistors (to 3.3V).
  10k resistors are on the EMSENSR-9250 breakout board.

  Hardware setup:
  MPU9250 Breakout --------- Arduino
  VDD ---------------------- 3.3V
  VDDI --------------------- 3.3V
  SDA ----------------------- A4
  SCL ----------------------- A5
  GND ---------------------- GND
*/

#include "src/quaternionFilters.h"
#include "src/MPU9250.h"

#define AHRS true // Set to false for basic data read
#define SerialDebug true // Set to true to get Serial output for debugging
#define magCal false
#define SerialSending false  // Set to true to get Serial output for debugging
#define SampleCount 480

#define test true

// Pin definitions
int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins
int myLed  = 13;  // Set up pin 13 led for toggling

// custom
int interval = 10;
float heading = 360;
float xh = 0;
float yh = 0;

#define I2Cclock 400000
#define I2Cport Wire
#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0   // Use either this line or the next to select which I2C address your device is using
//#define MPU9250_ADDRESS MPU9250_ADDRESS_AD1

MPU9250 myIMU(MPU9250_ADDRESS, I2Cport, I2Cclock);



void setup()
{
  Wire.begin();
  TWBR = 12;  // 400 kbit/sec I2C speed
  Serial.begin(38400);

  while (!Serial) {};

  // Set up the interrupt pin, its set as active high, push-pull
  pinMode(intPin, INPUT);
  digitalWrite(intPin, LOW);
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);


  // Read the WHO_AM_I register, this is a good test of communication
  byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);

  if (SerialDebug) {
    Serial.print(F("MPU9250 I AM 0x"));
    Serial.print(c, HEX);
    Serial.print(F(" I should be 0x"));
    Serial.println(0x71, HEX);
  }



  if (c == 0x71) // WHO_AM_I should always be 0x71
  {
    if (SerialDebug) {
      Serial.println(F("MPU9250 is online..."));
      Serial.print(F("x-axis self test: acceleration trim within : "));
      Serial.print(myIMU.selfTest[0], 1); Serial.println("% of factory value");
      Serial.print(F("y-axis self test: acceleration trim within : "));
      Serial.print(myIMU.selfTest[1], 1); Serial.println("% of factory value");
      Serial.print(F("z-axis self test: acceleration trim within : "));
      Serial.print(myIMU.selfTest[2], 1); Serial.println("% of factory value");
      Serial.print(F("x-axis self test: gyration trim within : "));
      Serial.print(myIMU.selfTest[3], 1); Serial.println("% of factory value");
      Serial.print(F("y-axis self test: gyration trim within : "));
      Serial.print(myIMU.selfTest[4], 1); Serial.println("% of factory value");
      Serial.print(F("z-axis self test: gyration trim within : "));
      Serial.print(myIMU.selfTest[5], 1); Serial.println("% of factory value");

    }

    // Start by performing self test and reporting values
    myIMU.MPU9250SelfTest(myIMU.selfTest);



    // Calibrate gyro and accelerometers, load biases in bias registers
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);


    myIMU.initMPU9250();


    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    if (SerialDebug) {
      Serial.println("MPU9250 initialized for active data mode....");
    }


    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    if (SerialDebug) {
      Serial.print("AK8963 ");
      Serial.print("I AM 0x");
      Serial.print(d, HEX);
      Serial.print(" I should be 0x");
      Serial.println(0x48, HEX);

    }



    if (d != 0x48)
    {
      if (SerialDebug) {
        // Communication failed, stop here
        Serial.println(F("Communication failed, abort!"));
        Serial.flush();
      }
      abort();

    }

    // Get magnetometer calibration from AK8963 ROM
    myIMU.initAK8963(myIMU.factoryMagCalibration);
    // Initialize device for active mode read of magnetometer
    //    Serial.println("AK8963 initialized for active data mode....");

    if (SerialDebug)
    {
      //  Serial.println("Calibration values: ");
      Serial.print("X-Axis factory sensitivity adjustment value ");
      Serial.println(myIMU.factoryMagCalibration[0], 2);
      Serial.print("Y-Axis factory sensitivity adjustment value ");
      Serial.println(myIMU.factoryMagCalibration[1], 2);
      Serial.print("Z-Axis factory sensitivity adjustment value ");
      Serial.println(myIMU.factoryMagCalibration[2], 2);
    }



    // Get sensor resolutions, only need to do this once
    myIMU.getAres();
    myIMU.getGres();
    myIMU.getMres();

    if (magCal) {
      // magnetometer calibration with sampleCount for getting mag min/max
      // The next call delays for 4 seconds, and then records about 15 seconds of
      // data to calculate bias and scale.
      myIMU.magCalMPU9250(myIMU.magBias, myIMU.magScale, SampleCount);
    }

    if (SerialDebug)
    {
      Serial.println("AK8963 mag biases (mG)");
      Serial.println(myIMU.magBias[0]);
      Serial.println(myIMU.magBias[1]);
      Serial.println(myIMU.magBias[2]);

      Serial.println("AK8963 mag scale (mG)");
      Serial.println(myIMU.magScale[0]);
      Serial.println(myIMU.magScale[1]);
      Serial.println(myIMU.magScale[2]);
    }
    //    delay(2000); // Add delay to see results before serial spew of data

    if (SerialDebug)
    {
      Serial.println("Magnetometer:");
      Serial.print("X-Axis sensitivity adjustment value ");
      Serial.println(myIMU.factoryMagCalibration[0], 2);
      Serial.print("Y-Axis sensitivity adjustment value ");
      Serial.println(myIMU.factoryMagCalibration[1], 2);
      Serial.print("Z-Axis sensitivity adjustment value ");
      Serial.println(myIMU.factoryMagCalibration[2], 2);
    }


  } // if (c == 0x71)
  else
  {
    Serial.print("Could not connect to MPU9250: 0x");
    Serial.println(c, HEX);

    // Communication failed, stop here
    Serial.println(F("Communication failed, abort!"));
    Serial.flush();
    abort();
  }
}

void loop()
{
  // If intPin goes high, all data registers have new data
  // On interrupt, check if data ready interrupt
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    myIMU.ax = (float)myIMU.accelCount[0] * myIMU.aRes; // - myIMU.accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1] * myIMU.aRes; // - myIMU.accelBias[1];
    myIMU.az = (float)myIMU.accelCount[2] * myIMU.aRes; // - myIMU.accelBias[2];

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    myIMU.gx = (float)myIMU.gyroCount[0] * myIMU.gRes;
    myIMU.gy = (float)myIMU.gyroCount[1] * myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2] * myIMU.gRes;

    myIMU.readMagData(myIMU.magCount);  // Read the x/y/z adc values

    // use calculated data
    if (!magCal) {

//      myIMU.magScale[0] = 1.00;
//      myIMU.magScale[1] = 0.99;
//      myIMU.magScale[2] = 1.01;
//
//      myIMU.magBias[0] = +252.18 * myIMU.magScale[0];  // User environmental x-axis correction in milliGauss, should be automatically calculated
//      myIMU.magBias[1] = +23.07 * myIMU.magScale[1];  // User environmental x-axis correction in milliGauss
//      myIMU.magBias[2] = -326.12 * myIMU.magScale[2];  // User environmental x-axis correction in milliGauss


      // myIMU.magScale[0] = 1.07;
      // myIMU.magScale[1] = 0.99;
      // myIMU.magScale[2] = 0.94;

      // myIMU.magBias[0] = +315.22 * myIMU.magScale[0];  // User environmental x-axis correction in milliGauss, should be automatically calculated
      // myIMU.magBias[1] = -55.01 * myIMU.magScale[1];  // User environmental x-axis correction in milliGauss
      // myIMU.magBias[2] = -373.68 * myIMU.magScale[2];  // User environmental x-axis correction in milliGauss


      myIMU.magScale[0] = 1.00;
      myIMU.magScale[1] = 1.00;
      myIMU.magScale[2] = 1.00;

      myIMU.magBias[0] = 316.97 * myIMU.magScale[0];  // User environmental x-axis correction in milliGauss, should be automatically calculated
      myIMU.magBias[1] = +24.85 * myIMU.magScale[1];  // User environmental x-axis correction in milliGauss
      myIMU.magBias[2] = -276.86 * myIMU.magScale[2];  // User environmental x-axis correction in milliGauss



    }


    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set
    myIMU.mx = (float)myIMU.magCount[0] * myIMU.mRes
               * myIMU.factoryMagCalibration[0] - myIMU.magBias[0];
    myIMU.my = (float)myIMU.magCount[1] * myIMU.mRes
               * myIMU.factoryMagCalibration[1] - myIMU.magBias[1];
    myIMU.mz = (float)myIMU.magCount[2] * myIMU.mRes
               * myIMU.factoryMagCalibration[2] - myIMU.magBias[2];
  } // if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)

  // Must be called before updating quaternions!
  myIMU.updateTime();

  // Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
  // the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
  // (+ up) of accelerometer and gyro! We have to make some allowance for this
  // orientationmismatch in feeding the output to the quaternion filter. For the
  // MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward
  // along the x-axis just like in the LSM9DS0 sensor. This rotation can be
  // modified to allow any convenient orientation convention. This is ok by
  // aircraft orientation standards! Pass gyro rate as rad/s
  // MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx * DEG_TO_RAD,
  //                        myIMU.gy * DEG_TO_RAD, myIMU.gz * DEG_TO_RAD, myIMU.my,
  //                        myIMU.mx, myIMU.mz, myIMU.deltat);


   MadgwickQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx * DEG_TO_RAD,
                          myIMU.gy * DEG_TO_RAD, myIMU.gz * DEG_TO_RAD, myIMU.my,
                          myIMU.mx, myIMU.mz, myIMU.deltat);


  if (!AHRS)
  {
    myIMU.delt_t = millis() - myIMU.count;
    if (myIMU.delt_t > interval)
    {
      if (SerialDebug)
      {
        // Print acceleration values in milligs!
        Serial.print("X-acceleration: "); Serial.print(1000 * myIMU.ax);
        Serial.print(" mg ");
        Serial.print("Y-acceleration: "); Serial.print(1000 * myIMU.ay);
        Serial.print(" mg ");
        Serial.print("Z-acceleration: "); Serial.print(1000 * myIMU.az);
        Serial.println(" mg ");

        // Print gyro values in degree/sec
        Serial.print("X-gyro rate: "); Serial.print(myIMU.gx, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Y-gyro rate: "); Serial.print(myIMU.gy, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Z-gyro rate: "); Serial.print(myIMU.gz, 3);
        Serial.println(" degrees/sec");

        // Print mag values in degree/sec
        Serial.print("X-mag field: "); Serial.print(myIMU.mx);
        Serial.print(" mG ");
        Serial.print("Y-mag field: "); Serial.print(myIMU.my);
        Serial.print(" mG ");
        Serial.print("Z-mag field: "); Serial.print(myIMU.mz);
        Serial.println(" mG");

        // myIMU.tempCount = myIMU.readTempData();  // Read the adc values
        // Temperature in degrees Centigrade
        // myIMU.temperature = ((float) myIMU.tempCount) / 333.87 + 21.0;
        // Print temperature in degrees Centigrade
        // Serial.print("Temperature is ");  Serial.print(myIMU.temperature, 1);
        // Serial.println(" degrees C");
      }



      myIMU.count = millis();
      digitalWrite(myLed, !digitalRead(myLed));  // toggle led
    } // if (myIMU.delt_t > 500)
  } // if (!AHRS)
  else
  {
    // Serial print and/or display at 0.5 s rate independent of data rates
    myIMU.delt_t = millis() - myIMU.count;

    // update LCD once per half-second independent of read rate
    if (myIMU.delt_t > interval)
    {
      if (SerialDebug)
      {
        // Serial.print("ax = ");  Serial.print((int)1000 * myIMU.ax);
        // Serial.print(" ay = "); Serial.print((int)1000 * myIMU.ay);
        // Serial.print(" az = "); Serial.print((int)1000 * myIMU.az);
        // Serial.println(" mg");

        // Serial.print("gx = ");  Serial.print(myIMU.gx, 2);
        // Serial.print(" gy = "); Serial.print(myIMU.gy, 2);
        // Serial.print(" gz = "); Serial.print(myIMU.gz, 2);
        // Serial.println(" deg/s");

        // Serial.print("mx = ");  Serial.print((int)myIMU.mx);
        // Serial.print(" my = "); Serial.print((int)myIMU.my);
        // Serial.print(" mz = "); Serial.print((int)myIMU.mz);
        // Serial.println(" mG");

        // Serial.print("q0 = ");  Serial.print(*getQ());
        // Serial.print(" qx = "); Serial.print(*(getQ() + 1));
        // Serial.print(" qy = "); Serial.print(*(getQ() + 2));
        // Serial.print(" qz = "); Serial.println(*(getQ() + 3));
      }

      // Define output variables from updated quaternion---these are Tait-Bryan
      // angles, commonly used in aircraft orientation. In this coordinate system,
      // the positive z-axis is down toward Earth. Yaw is the angle between Sensor
      // x-axis and Earth magnetic North (or true North if corrected for local
      // declination, looking down on the sensor positive yaw is counterclockwise.
      // Pitch is angle between sensor x-axis and Earth ground plane, toward the
      // Earth is positive, up toward the sky is negative. Roll is angle between
      // sensor y-axis and Earth ground plane, y-axis up is positive roll. These
      // arise from the definition of the homogeneous rotation matrix constructed
      // from quaternions. Tait-Bryan angles as well as Euler angles are
      // non-commutative; that is, the get the correct orientation the rotations
      // must be applied in the correct order which for this configuration is yaw,
      // pitch, and then roll.
      // For more see
      // http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
      // which has additional links.
      myIMU.yaw   = atan2(2.0f * (*(getQ() + 1) * *(getQ() + 2) + *getQ()
                                  * *(getQ() + 3)), *getQ() * *getQ() + * (getQ() + 1)
                          * *(getQ() + 1) - * (getQ() + 2) * *(getQ() + 2) - * (getQ() + 3)
                          * *(getQ() + 3));
      myIMU.pitch = -asin(2.0f * (*(getQ() + 1) * *(getQ() + 3) - *getQ()
                                  * *(getQ() + 2)));
      myIMU.roll  = atan2(2.0f * (*getQ() * *(getQ() + 1) + * (getQ() + 2)
                                  * *(getQ() + 3)), *getQ() * *getQ() - * (getQ() + 1)
                          * *(getQ() + 1) - * (getQ() + 2) * *(getQ() + 2) + * (getQ() + 3)
                          * *(getQ() + 3));
      // myIMU.pitch *= RAD_TO_DEG;
      myIMU.yaw   *= RAD_TO_DEG;

      // Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
      // 	8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
      // - http://www.ngdc.noaa.gov/geomag-web/#declination
      myIMU.yaw  += 8.99;
      // myIMU.roll *= RAD_TO_DEG;

      // heading = atan2(myIMU.my, myIMU.mx) * RAD_TO_DEG;

      // float x = -myIMU.gy;
      // float y = myIMU.gx;

      // float phi = -myIMU.roll;
      // float theta = myIMU.pitch;
      // double pitch = atan2 (myIMU.ay ,( sqrt ((myIMU.ax * myIMU.ax) + (myIMU.az * myIMU.az))));
      // double roll = atan2(-myIMU.ax,( sqrt((myIMU.ay* myIMU.ay) + (myIMU.az* myIMU.az))));

      // double pitch = atan2(myIMU.ax, sqrt(myIMU.ay*myIMU.ay + myIMU.az*myIMU.az));
      // double roll = atan2(-myIMU.ay, myIMU.az);

      // float Yh = (myIMU.my * cos(roll)) - (myIMU.mz * sin(roll));
      // float Xh = (myIMU.mx * cos(pitch))+(myIMU.my* sin(roll)*sin(pitch)) + (myIMU.mz* cos(roll) * sin(pitch));
      // heading = atan2(Yh, Xh);

  //Low Pass Filter
  float alpha = 0.2;
  float fax = myIMU.ax * alpha + (fax * (1.0 - alpha));
  float fay = myIMU.ay * alpha + (fay * (1.0 - alpha));
  float faz = myIMU.az * alpha + (faz * (1.0 - alpha));

  float accxnorm = fax/sqrt(fax*fax+fay*fay+faz*faz);
  float accynorm = fay/sqrt(fax*fax+fay*fay+faz*faz);

  // Normalize acceleration measurements so they range from 0 to 1
  // float accxnorm = myIMU.ax/sqrt(myIMU.ax*myIMU.ax+myIMU.ay*myIMU.ay+myIMU.az*myIMU.az);
  // float accynorm = myIMU.ay/sqrt(myIMU.ax*myIMU.ax+myIMU.ay*myIMU.ay+myIMU.az*myIMU.az);

  // calculate pitch and roll
  float pitch = asin(-accxnorm);
  float roll = asin(accynorm/cos(pitch));

  // tilt compensated magnetic sensor measurements
  float magxcomp = myIMU.mx*cos(pitch)+myIMU.mz*sin(pitch);
  float magycomp = myIMU.mx*sin(roll)*sin(pitch)+myIMU.my*cos(roll)-myIMU.mz*sin(roll)*cos(pitch);

  // arctangent of y/x converted to degrees
  heading = 180*atan2(magycomp,magxcomp)/PI;


      // xh = x * cos(myIMU.pitch) * y * sin(myIMU.roll) * sin(myIMU.pitch) - myIMU.mz * cos(myIMU.roll) * sin(myIMU.pitch);
      // yh = y * cos(-myIMU.roll) + myIMU.mz * sin(myIMU.roll);
      // heading = atan2(yh, xh) * RAD_TO_DEG;
      // heading = atan2(-(myIMU.mz * sin(phi) - myIMU.my * cos(phi)), myIMU.mx * cos(theta) + myIMU.my * sin(theta) * sin(phi) + myIMU.mz * sin(theta) * cos(phi));
      // heading *= RAD_TO_DEG;
      pitch *= RAD_TO_DEG;
      roll *= RAD_TO_DEG;

      if (test) {
        Serial.print(pitch);
        Serial.print(" ");
        Serial.print(roll);
        Serial.print(" ");
        Serial.println(heading);

      }

      if (SerialSending)
      {
        // float r[] = {myIMU.mx, myIMU.my, myIMU.mz, myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx, myIMU.gy, myIMU.gz, myIMU.yaw, myIMU.pitch, myIMU.roll};

        // Serial.println(r);

        // mag
        Serial.print("s");
        Serial.print(" ");
        Serial.print(myIMU.mx, 3);
        Serial.print(" ");
        Serial.print(myIMU.my, 3);
        Serial.print(" ");
        Serial.print(myIMU.mz, 3);

        // acc
        Serial.print(" ");
        Serial.print(myIMU.ax, 3);
        Serial.print(" ");
        Serial.print(myIMU.ay, 3);
        Serial.print(" ");
        Serial.print(myIMU.az, 3);

        // gyro
        Serial.print(" ");
        Serial.print(myIMU.gx, 3);
        Serial.print(" ");
        Serial.print(myIMU.gy, 3);
        Serial.print(" ");
        Serial.print(myIMU.gz, 3);

        //    Serial.print("Yaw, Pitch, Roll: ");
        Serial.print(" ");
        Serial.print(myIMU.yaw, 3);
        Serial.print(" ");
        Serial.print(myIMU.pitch, 3);
        Serial.print(" ");
        Serial.print(myIMU.roll, 3);
        Serial.print(" ");


        Serial.print(* getQ(), 3);
        Serial.print(" ");
        Serial.print(* getQ() + 1, 3);
        Serial.print(" ");
        Serial.print(* getQ() + 2, 3);
        Serial.print(" ");
        Serial.print(* getQ() + 3, 3);
        Serial.print(" ");


        Serial.print("e");
        Serial.println(" ");

      }



      myIMU.count = millis();
      myIMU.sumCount = 0;
      myIMU.sum = 0;
    } // if (myIMU.delt_t > 500)
  } // if (AHRS)
}
