/*
 *  main.ino
 *
 *  Created on: 03/10/2022
 *      Author: Muhammad Danish
 *  
 *  Reference : https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
 */
 
#define PWR_MGMT_1 0x6B  //Constant

// specific I2C addresses may be set as a defines here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
#define MPU6050_ADDR 0x68 

// Full scale selection 
// gyroscope register configuration 
// gyroscope register Address is 0x1B
#define GYFS_ADDR 0x1B   //Constant
// FS_SEL full scale selection bits
// setting bits are 0b000xx000
// 0x00 || 0b00000000 for +-250  °/s
// 0x08 || 0b00001000 for +-500  °/s
// 0x10 || 0b00010000 for +-1000 °/s
// 0x18 || 0b00011000 for +-2000 °/s
#define FS_SEL 0x00 

// Full scale selection 
// Accelerometer register configuration 
// Accelerometer register Address is 0x1C  
#define ACCEL_ADDR 0x1C   //Constant
// AFS_SEL Acceleration full scale selection bits
// setting bits are 0b000xx000
// 0x00 || 0b00000000 for +-2g
// 0x08 || 0b00001000 for +-4g
// 0x10 || 0b00010000 for +-8g
// 0x18 || 0b00011000 for +-16g
#define AFS_SEL 0x00

//ENABLE or DISABLE TEMPERATURE SENSOR
#define ENABLE_TEMP    //uncomment will disable the temperature sensor
#ifndef ENABLE_TEMP
  #define DISABLE_TEMP
#endif


// Accelerometer Measurements Starting Register
#define ACCEL_rREG 0x3B  //constant
// Temperature Measurements Starting Register
#define TEMP_rREG  0x41   //constant
// Accelerometer Measurements Starting Register
#define GYRO_rREG  0x43   //constant
