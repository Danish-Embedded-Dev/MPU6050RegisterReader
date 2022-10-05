#include"conf.h"
#include"main.h"

void setup() { 
 //Start I2C communication  
 Wire.begin();      

 //
 Serial.begin(9600);

 //Setup the registers of the MPU-6050 (500dfs / +/-8g) 
 //and start the gyro
 setup_mpu_6050_registers();  

//  Read_buffer(MPU6050_ADDR,PWR_MGMT_1,1);
 Read_buffer(MPU6050_ADDR,ACCEL_ADDR,1);
 Read_buffer(MPU6050_ADDR,GYFS_ADDR,1);
}

void loop() {

Read_buffer(MPU6050_ADDR,0x72,2);  
  // put your main code here, to run repeatedly:
//  char SerialPayload[50];   //using for serial data 
  
//  Serial.println(Wire.read());
//    Read_buffer(MPU6050_ADDR,PWR_MGMT_1,1);
//  Read_buffer(MPU6050_ADDR,0x3B,6);
//  Read_buffer(MPU6050_ADDR,0x1A,1);

//
recordAccelRegisters();
//sprintf(SerialPayload, "AccelX = %.2f, AccelY = %.2f, AccelZ = %.2f ", accel.x,accel.y,accel.z); 
////Serial.println(SerialPayload);
//Serial.print("  accX ="); Serial.print(accel.x);
//Serial.print("  accY ="); Serial.print(accel.y);
//Serial.print("  accZ ="); Serial.println(accel.z);  
////
//Serial.print("  accX ="); Serial.print(accel_raw.x);
//Serial.print("  accY ="); Serial.print(accel_raw.y); 
//Serial.print("  accZ ="); Serial.println(accel_raw.z);    

recordGyroRegisters();
//Serial.print("  gyroX :"); Serial.print(gyro.x);
//Serial.print(",  gyroY :"); Serial.print(gyro.y);
//Serial.print(",  gyroZ :"); Serial.print(gyro.z); 
//Serial.println("") ;
//
//Serial.print(", gyroX :"); Serial.print(gyro_raw.x);
//Serial.print(",  gyroY :"); Serial.print(gyro_raw.y); 
//Serial.print(",  gyroZ :"); Serial.println(gyro_raw.z);    
  delay(100);
}


void Read_buffer(byte DevAddr,byte RegAddr, byte read_nbytes){
  char SerialPayload[15];   //using for serial data 
  
  Wire.beginTransmission(DevAddr);                                    //Start communicating with the MPU-6050
  Wire.write(RegAddr);                                                //Send the requested starting register
  Wire.endTransmission();                                             //End the transmission
  Wire.requestFrom(DevAddr,read_nbytes);                               //Request n-bytes from the MPU-6050
  while(Wire.available() < read_nbytes);                               //Wait until all the bytes are received


  for(int i = 0; i < read_nbytes ; i++){
    sprintf(SerialPayload, "byte[%02X] = ", RegAddr+i);
    if(read_nbytes == 1){  //read in binary format when 1 byte read else in hexadecimal representation
      Serial.print(SerialPayload);  Serial.println(Wire.read(),BIN); 
    }else{
      Serial.print(SerialPayload);  Serial.println(Wire.read(),HEX);   
    }
  }
}

void setup_mpu_6050_registers(){
  //Activate the MPU-6050
  Wire.beginTransmission(MPU6050_ADDR);                                //Start communicating with the MPU-6050
  Wire.write(PWR_MGMT_1);                                              //Send the requested starting register
  Wire.write(0x00);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  
  //Configure the accelerometer 
  Wire.beginTransmission(MPU6050_ADDR);                                //Start communicating with the MPU-6050
  Wire.write(ACCEL_ADDR);                                              //Send the requested starting register
  Wire.write(AFS_SEL);                                                 //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  
  //Configure the gyro 
  Wire.beginTransmission(MPU6050_ADDR);                                //Start communicating with the MPU-6050
  Wire.write(GYFS_ADDR);                                               //Send the requested starting register
  Wire.write(FS_SEL);                                                  //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission

  //enable or disable the temperature sensor
  Wire.beginTransmission(MPU6050_ADDR);                                //Start communicating with the MPU-6050
  Wire.write(PWR_MGMT_1);                                              //Send the requested starting register
#ifdef ENABLE_TEMP
  Wire.write(0x00);                                                    //Clear the temp bit to enable the temperature module
#endif
#ifdef DISABLE_TEMP
  Wire.write(0x08);                                                    //Set the temp bit to disable the temperature module
#endif
  Wire.endTransmission();                                              //End the transmission
  
}


void recordAccelRegisters() {
  Wire.beginTransmission(MPU6050_ADDR);                               //Start communicating with the MPU-6050
  Wire.write(ACCEL_rREG);                                             //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(MPU6050_ADDR,6);                                   //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accel_raw.x = Wire.read()<<8|Wire.read();                                //Store first two bytes into accelX
  accel_raw.y = Wire.read()<<8|Wire.read();                                //Store middle two bytes into accelY
  accel_raw.z = Wire.read()<<8|Wire.read();                                //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData(){
  float Sensitivity ;
  switch(AFS_SEL) {
  case 0x00:
    Sensitivity = 16384;
    break;
  case 0x08:
    Sensitivity = 8192;
    break;
  case 0x10:
    Sensitivity = 4096;
    break;  
  case 0x18:
    Sensitivity = 2048;
    break;   
}
  accel.x = (float)accel_raw.x / Sensitivity;
  accel.y = (float)accel_raw.y / Sensitivity; 
  accel.z = (float)accel_raw.z / Sensitivity;
}


void recordGyroRegisters() {
  Wire.beginTransmission(MPU6050_ADDR);                                    //Start communicating with the MPU-6050
  Wire.write(GYRO_rREG);                                                   //Starting register for Gyroscope Readings
  Wire.endTransmission();
  Wire.requestFrom(MPU6050_ADDR,6);                                        //Request gyroscope Registers (3B - 40)
  while(Wire.available() < 6);
  gyro_raw.x = Wire.read()<<8|Wire.read();                                //Store first two bytes into  gyroX
  gyro_raw.y = Wire.read()<<8|Wire.read();                                //Store middle two bytes into gyroY
  gyro_raw.z = Wire.read()<<8|Wire.read();                                //Store last two bytes into  gyroZ
  processGyroData();
}

void processGyroData(){
  float Sensitivity =131 ;
//  switch(FS_SEL) {
//  case 0x00:
//    Sensitivity = 131;
//    break;
//  case 0x08:
//    Sensitivity = 65.5;
//    break;
//  case 0x10:
//    Sensitivity = 32.8;
//    break;  
//  case 0x18:
//    Sensitivity = 16.4;
//    break;   
//  }
  gyro.x = (float)gyro_raw.x / Sensitivity;
  gyro.y = (float)gyro_raw.y / Sensitivity; 
  gyro.z = (float)gyro_raw.z / Sensitivity;
}
