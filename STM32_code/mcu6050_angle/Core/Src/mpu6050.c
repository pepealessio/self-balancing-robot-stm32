/**
  ******************************************************************************
  * @file           : mpu6050.h
  * @brief          : mpu6050 driver declaration restricted on principal
  * 				  function and for a SBR.
  * @version		: v1.0
  ******************************************************************************
  */

/********************* INCLUDE ******************************/
#include <math.h>
#include "mpu6050.h"
/********************* end INCLUDE ******************************/

/********************* DEFINE ******************************/
#define RAD_TO_DEG					(57.295779513082320876798154814105)

#define PWR_MGMT_1_REG 				(0x6B)
#define SMPLRT_DIV_REG 				(0x19)
#define ACCEL_CONFIG_REG 			(0x1C)
#define ACCEL_XOUT_H_REG 			(0x3B)
#define GYRO_CONFIG_REG 			(0x1B)
#define MPU6050_ADDR 				(0xD0) 		/* (0x68 << 1) */
#define MPU6050__I2C_TIMEOUT 		(100)
#define MPU6050__START_TRIAL		(1000)
/********************* end DEFINE **************************/

/********************* FUNCTION ******************************/
HAL_StatusTypeDef mpu6050__init(mpu6050_t *hmpu6050, I2C_HandleTypeDef *hi2c)
{
	hmpu6050->hi2c = hi2c;
	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hmpu6050->hi2c, MPU6050_ADDR, MPU6050__START_TRIAL, HAL_MAX_DELAY);
	if (status == HAL_OK)
	{
		status = mpu6050__begin(hmpu6050);
	}

	return status;
}

HAL_StatusTypeDef mpu6050__begin(mpu6050_t *hmpu6050)
{
	// power management register 0X6B we should write all 0's to wake the sensor up (reset all)
	hmpu6050->data = 0x00;
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hmpu6050->hi2c, MPU6050_ADDR, PWR_MGMT_1_REG, I2C_MEMADD_SIZE_8BIT, &hmpu6050->data, 1, MPU6050__I2C_TIMEOUT);
	if (status != HAL_OK) {return status;}

	// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
	hmpu6050->data = 0x07;
	status = HAL_I2C_Mem_Write(hmpu6050->hi2c, MPU6050_ADDR, SMPLRT_DIV_REG, I2C_MEMADD_SIZE_8BIT, &hmpu6050->data, 1, MPU6050__I2C_TIMEOUT);
	if (status != HAL_OK) {return status;}

	// Set accelerometer configuration in ACCEL_CONFIG Register
	// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> 2 g
	hmpu6050->data = 0x00;
	status = HAL_I2C_Mem_Write(hmpu6050->hi2c, MPU6050_ADDR, ACCEL_CONFIG_REG, I2C_MEMADD_SIZE_8BIT, &hmpu6050->data, 1, MPU6050__I2C_TIMEOUT);
	if (status != HAL_OK) {return status;}

	// Set Gyroscope configuration in GYRO_CONFIG Register
	// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> 250 deg/s
	hmpu6050->data = 0x00;
	status = HAL_I2C_Mem_Write(hmpu6050->hi2c, MPU6050_ADDR, GYRO_CONFIG_REG, I2C_MEMADD_SIZE_8BIT, &hmpu6050->data, 1, MPU6050__I2C_TIMEOUT);

#if defined (MPU6050__ENABLE_X)
	hmpu6050->__complementary_angle_X = 0;
#endif

#if defined (MPU6050__ENABLE_Y)
	hmpu6050->__complementary_angle_Y = 0;
#endif

	return status;
}

HAL_StatusTypeDef mpu6050__read_all(mpu6050_t *hmpu6050)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hmpu6050->hi2c, MPU6050_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, hmpu6050->buffer, MPU6050__RX_BUFFER_SIZE, MPU6050__I2C_TIMEOUT);

#if defined (MPU6050__ENABLE_Y)
	hmpu6050->acc_X_raw = 	(int16_t) (hmpu6050->buffer[0] << 8 | hmpu6050->buffer[1]);
#endif

#if defined (MPU6050__ENABLE_X)
	hmpu6050->acc_Y_raw = 	(int16_t) (hmpu6050->buffer[2] << 8 | hmpu6050->buffer[3]);
#endif

	hmpu6050->acc_Z_raw = 	(int16_t) (hmpu6050->buffer[4] << 8 | hmpu6050->buffer[5]);

#if defined (ENABLE_TEMPERATURE)
	hmpu6050->temp_raw = 	(int16_t) (hmpu6050->buffer[6] << 8 | hmpu6050->buffer[7]);
#endif

#if defined (MPU6050__ENABLE_X)
	hmpu6050->gyro_X_raw = 	(int16_t) (hmpu6050->buffer[8] << 8 | hmpu6050->buffer[9]);
#endif

#if defined (MPU6050__ENABLE_Y)
	hmpu6050->gyro_Y_raw = 	(int16_t) (hmpu6050->buffer[10] << 8 | hmpu6050->buffer[11]);
#endif

#if defined (MPU6050__ENABLE_GYRO_Z)
	hmpu6050->gyro_Z_raw = 	(int16_t) (hmpu6050->buffer[12] << 8 | hmpu6050->buffer[13]);
#endif

	/*** convert the RAW values into acceleration in 'g'
		 we have to divide according to the Full scale value set in FS_SEL
		 I have configured FS_SEL = 0. So I am dividing by 16384.0
		 for more details check ACCEL_CONFIG Register              ****/
#if defined (MPU6050__SEE_REFINED_ACC_XYZ)
#if defined (MPU6050__ENABLE_Y)
	hmpu6050->acc_X = hmpu6050->acc_X_raw / 16384.0;
#endif

#if defined (MPU6050__ENABLE_X)
	hmpu6050->acc_Y = hmpu6050->acc_Y_raw / 16384.0;
#endif

	hmpu6050->acc_Z = hmpu6050->acc_Z_raw / 16384.0;
#endif

#if defined (ENABLE_TEMPERATURE)
	hmpu6050->temp = (float) ((int16_t) hmpu6050->temp_raw / (float) 340.0 + (float) 36.53);
#endif

	/*** convert the RAW values into dps (deg/s)
		 we have to divide according to the Full scale value set in FS_SEL
		 I have configured FS_SEL = 0. So I am dividing by 131.0
		 for more details check GYRO_CONFIG Register              ****/
#if defined (MPU6050__ENABLE_X)
	hmpu6050->gyro_X = hmpu6050->gyro_X_raw / 131.0;
#endif

#if defined (MPU6050__ENABLE_Y)
	hmpu6050->gyro_Y = hmpu6050->gyro_Y_raw / 131.0;
#endif

#if defined (MPU6050__ENABLE_GYRO_Z)
	hmpu6050->gyro_Z = hmpu6050->gyro_Z_raw / 131.0;
#endif

#if defined (MPU6050__ENABLE_X)
	__mpu6050__getangleX(hmpu6050);
#endif

#if defined (MPU6050__ENABLE_Y)
	__mpu6050__getangleY(hmpu6050);
#endif

	return status;
}

#if defined (MPU6050__ENABLE_X)
void __mpu6050__getangleX(mpu6050_t *hmpu6050)
{
	double gyro_angle, acc_angle;

	// Get X current angle with accelerometer
	acc_angle = atan2(hmpu6050->acc_Y_raw, hmpu6050->acc_Z_raw) * RAD_TO_DEG;

	// Get X current angle with gyroscope
	gyro_angle = (double) hmpu6050->gyro_X * MPU6050__DT;

	// Evaluate complementary angle
	hmpu6050->__complementary_angle_X = MPU6050__ALPHA * (hmpu6050->__complementary_angle_X + gyro_angle) + (1 - MPU6050__ALPHA) * acc_angle;
	hmpu6050->complementary_angle_X = hmpu6050->__complementary_angle_X + MPU6050__angle_offset_X;
}
#endif

#if defined (MPU6050__ENABLE_Y)
void __mpu6050__getangleY(mpu6050_t *hmpu6050)
{
	double gyro_angle, acc_angle;

	// Get Y current angle with accelerometer
	acc_angle = atan2(hmpu6050->acc_X_raw, hmpu6050->acc_Z_raw) * RAD_TO_DEG;

	// Get Y current angle with gyroscope
	gyro_angle = (double) hmpu6050->gyro_Y * MPU6050__DT;

	// Evaluate complementary angle
	hmpu6050->__complementary_angle_Y = MPU6050__ALPHA * (hmpu6050->__complementary_angle_Y + gyro_angle) + (1 - MPU6050__ALPHA) * acc_angle;
	hmpu6050->complementary_angle_Y = hmpu6050->__complementary_angle_Y + MPU6050__angle_offset_Y;
}
#endif
/********************* end FUNCTION ******************************/
