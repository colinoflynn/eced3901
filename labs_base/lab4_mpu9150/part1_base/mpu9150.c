/* by Colin O'Flynn, based on the following:
 1) Post at http://www.avrfreaks.net/forum/i2c-twi-atmega1284p-mpu9150 which is based on an Arduino library.
 
 Released into the public domain (where possible).
*/

#include <avr/io.h>
#include <util/delay.h>
#include "twimaster.h"
#include "mpu9150.h"

/* Read one of the accelerometer or gyro axis registers */
int Read_Acc_Gyro(unsigned char axis)
{
  int Data;
   
  /* Select the required register */ 
  i2c_start_wait(I2CACCGYROADD+I2C_WRITE); 
  i2c_write(axis); 
  i2c_stop(); 
  
  /* Request the high and low bytes for the required axis */
  i2c_rep_start(I2CACCGYROADD+I2C_READ); 
  Data = i2c_readAck() << 8;
  Data = Data | i2c_readNak();
  i2c_stop(); 
  
  return Data;
}

/* Initialises the accelerometer and gyro to one of the sensitivity 
   ranges and puts the I2C bus into pass-through mode */
void Initalise_AccelGyro(unsigned char Accel_Range,unsigned char Gyro_Range)
{
  /* Take the MPU9150 out of sleep */
  i2c_start_wait(I2CACCGYROADD+I2C_WRITE);
  i2c_write(PWR_MGMT_1); 
  i2c_write(0); 
  i2c_stop(); 
  
  /* Set the sensitivity of the module */
  i2c_start_wait(I2CACCGYROADD+I2C_WRITE);
  i2c_write(ACCEL_CONFIG); 
  i2c_write(Accel_Range << 3); 
  i2c_stop(); 
  
  /* Set the sensitivity of the module */
  i2c_start_wait(I2CACCGYROADD+I2C_WRITE);
  i2c_write(GYRO_CONFIG); 
  i2c_write(Gyro_Range << 3); 
  i2c_stop(); 
  
  /* Put the I2C bus into pass-through mode so that the aux I2C interface
     that has the compass connected to it can be accessed */
  i2c_start_wait(I2CACCGYROADD+I2C_WRITE); 
  i2c_write(0x6A); 
  i2c_write(0x00); 
  i2c_stop();

  i2c_start_wait(I2CACCGYROADD+I2C_WRITE); 
  i2c_write(0x37); 
  i2c_write(0x02); 
  i2c_stop(); 
}

unsigned char compass_valid = 0;

/* Read one of the compass axis */
int Read_Compass(unsigned char axis)
{
  int Data;
  
  if(compass_valid == 0){
	  
	  Data = 0;
	  
	  //WARNING: This will LOCK if compass wasn't triggered
	  while((Data & 0x01) == 0){
	  
		  i2c_start_wait(I2CCOMPADD+I2C_WRITE);
		  i2c_write(COMP_STATUS);
		  i2c_stop();
	  
  
		  /* Request the high and low bytes for the required axis */
		  i2c_rep_start(I2CCOMPADD+I2C_READ);
		  Data = i2c_readNak();
		  i2c_stop();	
		  
		  _delay_us(500);
	  }
  }
  
  compass_valid = 1;
 
  /* Select the required axis register */
  i2c_start_wait(I2CCOMPADD+I2C_WRITE); 
  i2c_write(axis); 
  i2c_stop(); 
 
  /* Request the low and high bytes for the required axis */
  i2c_rep_start(I2CCOMPADD+I2C_READ); 
  Data = i2c_readAck();
  Data |= (i2c_readNak()<<8);
  i2c_stop(); 
  
  return Data;
}

/* Trigger a single shot compass reading of all three axis */
void Trigger_Compass(void)
{
  compass_valid = 0;
  
  /* Trigger a measurement */
  i2c_start_wait(I2CCOMPADD+I2C_WRITE); 
  i2c_write(0x0A); 
  i2c_write(0x01); 
  i2c_stop();
  
  /* Wait for the measurement to complete */
  /*
  do
  {
    i2c_start_wait(I2CCOMPADD+I2C_WRITE); 
    i2c_write(COMP_STATUS); 
    i2c_stop();
	 
    i2c_rep_start(I2CCOMPADD+I2C_READ); 
    i2c_readNak();
  }while(!i2c_readNak()); 
  */
}