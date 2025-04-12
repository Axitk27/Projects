#ifndef SENSOR_H
#define SENSOR_H

#include <Adafruit_LSM9DS1.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <cmath>

// Define a delay macro if needed
#define LSM9DS1_DELAY (100)

class read_data {
  private:
    float anglef_x_New,anglef_x_old = 0,anglef_y_old= 0,anglef_y_New,angle_x_M,angle_y_M;
    float dt,angleg_x_New = 0,angleg_y_New = 0,gyrox_read = 0,gyroy_read = 0,gyroz_read = 0;
    float angleag_x,angleag_y;
    float anglem_z_M,angle_x_rad,angle_y_rad;
    sensors_event_t a, m, g, temp;

  public:
    unsigned millisOld;
    read_data(Adafruit_LSM9DS1* x);
    void sensor_setup(Adafruit_LSM9DS1 &x);
    void data_read(Adafruit_LSM9DS1 &x, read_data &y);
    void data_display(read_data &y);
};

#endif // SENSOR_H
