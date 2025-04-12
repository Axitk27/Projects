#include "Sensor.h"
// 21 <-> SDA
// 22 <-> SCL

// Sensor initialization function
void sensor(Adafruit_LSM9DS1 &x)
{
  x.setupAccel(x.LSM9DS1_ACCELRANGE_2G, x.LSM9DS1_ACCELDATARATE_10HZ);
  x.setupMag(x.LSM9DS1_MAGGAIN_4GAUSS);
  x.setupGyro(x.LSM9DS1_GYROSCALE_245DPS);
}

// Constructor for read_data class
read_data::read_data(Adafruit_LSM9DS1 *x)
{
}

// Member function to setup sensor
void read_data::sensor_setup(Adafruit_LSM9DS1 &x)
{
  Serial.println("LSM9DS1 data read demo");
  // Try to initialise and warn if we couldn't detect the chip
  if (!x.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1)
      ;
  }
  Serial.println("Found LSM9DS1 9DOF");
  sensor(x);
}

void read_data::data_read(Adafruit_LSM9DS1 &x, read_data &y)
{
  // Read sensor data
  x.read();
  x.getEvent(&y.a, &y.m, &y.g, &y.temp);

  // Calculate angle based on acceleration and apply low-pass filter
  y.angle_y_M = (180 * atan(y.a.acceleration.x / y.a.acceleration.z)) / 3.14;
  y.angle_x_M = (180 * atan(y.a.acceleration.y / y.a.acceleration.z)) / 3.14;

  y.anglef_x_New = 0.9 * y.anglef_x_old + 0.1 * y.angle_x_M;
  y.anglef_y_New = 0.9 * y.anglef_y_old + 0.1 * y.angle_y_M;

  y.anglef_x_old = y.anglef_x_New;
  y.anglef_y_old = y.anglef_y_New;

  // Calibrate gyroscope data
  if (y.g.gyro.x < 0.20 && y.g.gyro.x > -0.20)
  {
    y.gyrox_read = 0;
  }
  else
  {
    y.gyrox_read = -y.g.gyro.x;
  }

  if (y.g.gyro.y < 0.20 && y.g.gyro.y > -0.20)
  {
    y.gyroy_read = 0;
  }
  else
  {
    y.gyroy_read = y.g.gyro.y;
  }

  if (y.g.gyro.z < 0.20 && y.g.gyro.z > -0.20)
  {
    y.gyroz_read = 0;
  }
  else
  {
    y.gyroz_read = -y.g.gyro.z;
  }

  // Calculate the time delta
  float dt = (millis() - y.millisOld) / 1000.0;
  y.millisOld = millis();

  // Calculate angle based on gyroscope data
  y.angleg_y_New =   y.angleg_y_New + y.gyroy_read * dt * (180 / 3.14);
  y.angleg_x_New =   y.angleg_x_New + y.gyrox_read * dt * (180 / 3.14);

  // Combine accelerometer and gyroscope data using complementary filter
  y.angleag_x = (y.angleg_x_New * 0.70) +  (y.angle_x_M * 0.3);
  y.angleag_y = (y.angleg_y_New * 0.70) +  (y.angle_y_M * 0.3);

  //calcultate the angle around the z axis rotation
  y.angle_x_rad = y.angle_x_M*(3.14/180);
  y.angle_y_rad = y.angle_y_M*(3.14/180);
  float Xm =  y.m.magnetic.x;
  //y.anglem_z_M = (180/3.14)*atan2((y.m.magnetic.y)*())/y.m.magnetic.x);

}

void read_data::data_display(read_data &y)
{
  Serial.print(y.a.acceleration.x);
  Serial.print(",");
  Serial.print(y.a.acceleration.y);
  Serial.print(",");
  Serial.print(y.a.acceleration.z);
  Serial.print(",");
  Serial.print(y.anglef_x_New);
  Serial.print(",");
  Serial.print(y.anglef_y_New);
  Serial.print(",");
  Serial.print(y.angle_x_M);
  Serial.print(",");
  Serial.print(y.angle_y_M);
  Serial.print(",");
  Serial.print(y.gyrox_read * 180 / 3.14);
  Serial.print(",");
  Serial.print(y.gyroy_read * 180 / 3.14);
  Serial.print(",");
  Serial.print(y.g.gyro.z * 180 / 3.14);
  Serial.print(",");
  Serial.print(y.angleg_x_New);
  Serial.print(",");
  Serial.print(y.angleg_y_New);
  Serial.print(",");
  Serial.print(y.angleag_x);
  Serial.print(",");
  Serial.print(y.angleag_y);
  Serial.print(",");
  Serial.print(y.anglem_z_M);
  Serial.print("\n");
}
