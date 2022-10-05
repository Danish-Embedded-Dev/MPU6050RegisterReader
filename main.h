//include library 

//use for I2c communication 
#include <Wire.h>





//global structure 

// struct with gyro and accel variable
struct IMU {
  int16_t  x;
  int16_t  y;
  int16_t  z;
} gyro_raw,accel_raw;

struct IMU_f{
  float  x;
  float  y;
  float  z;
} gyro,accel;
