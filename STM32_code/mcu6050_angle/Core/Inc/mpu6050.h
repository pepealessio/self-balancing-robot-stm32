/**
  ******************************************************************************
  * @file           : mpu6050.c
  * @brief          : mpu6050 driver implementation restricted on principal
  * 				  function and for a SBR.
  * @version		: v1.0
  ******************************************************************************
  */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

/********************* INCLUDE ******************************/
#include <stdint.h>
#include "i2c.h"
/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/

/**
 * The alpha constant evaluated with the following formula:
 *
 * alpha = tau / (tau + dt)
 */
#define MPU6050__ALPHA		(0.1278)

/**
 * The dt in the complementary angle formula (the sample time).
 * That have to be evaluate in accord to alpha and the formula
 * currentAngle = alpha * (previousAngle + gyroAngle) + (1 - alpha) * (accAngle)
 */
#define MPU6050__DT					(0.005)

// Uncomment the following line to compute the X angle.
//#define MPU6050__ENABLE_X

// Uncomment the following line to compute the Y angle.
#define MPU6050__ENABLE_Y

#if defined (MPU6050__ENABLE_X)
/**
 * Set this constant to have a fixed value when the SBL is at angle 0.
 * Ignore this if you want disable the X angle.
 */
#define MPU6050__angle_offset_X		(-1.9f)
#endif

#if defined (MPU6050__ENABLE_Y)
/**
 * Set this constant to have a fixed value when the SBL is at angle 0.
 * Ignore this if you want disable the Y angle.
 */
#define MPU6050__angle_offset_Y		(-3.2f)
#endif

// Uncomment the following line to compute the temperature sensor.
//#define ENABLE_TEMPERATURE

// Uncomment the following line to compute the giro_z value.
// This is not needed to compute complementary angle.
//#define MPU6050__ENABLE_GYRO_Z

// Uncomment the following line to compute the refined accelerometer value.
// This is not needed for the complementary angle.
//#define MPU6050__SEE_REFINED_ACC_XYZ

// Here the buffer dimension for reading register want defined in
// addiction to the previous declaration
#if defined(MPU6050__ENABLE_GYRO_Z)
#define MPU6050__RX_BUFFER_SIZE		(14)
#else
#define MPU6050__RX_BUFFER_SIZE		(12)
#endif

/********************* end DEFINE **************************/

/********************* TYPEDEF ******************************/

/**
 * This structure contains all the result read from the mpu6050
 * and the evaluated value. Contains also the actual angle evaluated
 * as complementary angle.
 *
 * @note	You can directly read value from the following variables,
 * 			without getter methods.
 *
 * @attention:	PLEASE NOT WRITE on the following variables. Use just
 * 				in read mode.
 */
typedef struct mpu6050_s
{
	I2C_HandleTypeDef *hi2c;

#if defined (MPU6050__ENABLE_X)
	int16_t acc_Y_raw;
#if defined (MPU6050__SEE_REFINED_ACC_XYZ)
	double acc_Y;
#endif

	double __complementary_angle_X;
	double complementary_angle_X;
#endif

#if defined (MPU6050__ENABLE_Y)
	int16_t acc_X_raw;
#if defined (MPU6050__SEE_REFINED_ACC_XYZ)
	double acc_X;
#endif

	double __complementary_angle_Y;
	double complementary_angle_Y;
#endif

	int16_t acc_Z_raw;
#if defined (MPU6050__SEE_REFINED_ACC_XYZ)
	double acc_Z;
#endif

#if defined (ENABLE_TEMPERATURE)
	uint16_t temp_raw;
	float temp;
#endif

#if defined (MPU6050__ENABLE_X)
	int16_t gyro_X_raw;
	double gyro_X;
#endif

#if defined (MPU6050__ENABLE_Y)
	int16_t gyro_Y_raw;
	double gyro_Y;
#endif

#if defined (MPU6050__ENABLE_GYRO_Z)
	int16_t gyro_Z_raw;
	double gyro_Z;
#endif

	// Supporting operation variable
	uint8_t data;
	uint8_t buffer[MPU6050__RX_BUFFER_SIZE];
} mpu6050_t;

/********************* end TYPEDEF **************************/

/********************* PROTOTYPE ******************************/

/**
 * Initialize the device and check the status. This function reset
 * the mpu6050 to factory reset, set the sample time to 1kHz and the
 * range of accelerometer and gyroscope respectively to +-2g and
 * +-250deg/s.
 *
 * @param hmpu6050:	the handler's pointer of mpu6050 to read and control.
 * @param hi2c:		the handler's pointer of I2C port who the mpu6050 is
 * 					connected to.
 *
 * @return	the device's status: HAL_OK if the device is connected and
 * 			work, other value if an error occur.
 */
HAL_StatusTypeDef mpu6050__init(mpu6050_t *hmpu6050, I2C_HandleTypeDef *hi2c);

/**
 * This function reset the mpu6050 to factory reset, set the sample
 * time to 1kHz and the range of accelerometer and gyroscope respectively
 * to +-2g and +-250deg/s.
 *
 * @param hmpu6050:	the handler's pointer of mpu6050 to read and control.
 *
 * @return	the device's status: HAL_OK if the device is connected and
 * 			work, other value if an error occur.
 */
HAL_StatusTypeDef mpu6050__begin(mpu6050_t *hmpu6050);

/**
 * This function real all data (in locking mode) and save that in
 * the mpu6050_t attributes. Furthermore, compute the legible value
 * and the two angles.
 *
 * @param hmpu6050:	the handler's pointer of mpu6050 to read and control.
 *
 * @return	the device's status: HAL_OK if the device is connected and
 * 			work, other value if an error occur.
 */
HAL_StatusTypeDef mpu6050__read_all(mpu6050_t *hmpu6050);

#if defined(MPU6050__ENABLE_X)
/**
 * This method is used in the public API to compute the X complementary
 * angle.
 *
 * @attention:	PLEASE NOT USE THAT EXPLICITELY, you can obtain some
 * 				insignificant values
 */
void __mpu6050__getangleX(mpu6050_t *hmpu6050);
#endif

#if defined(MPU6050__ENABLE_Y)
/**
 * This method is used in the public API to compute the Y complementary
 * angle.
 *
 * @attention:	PLEASE NOT USE THAT EXPLICITELY, you can obtain some
 * 				insignificant values
 */
void __mpu6050__getangleY(mpu6050_t *hmpu6050);
#endif

/********************* end PROTOTYPE **************************/

/********************* SINGLETON ******************************/

/**
 * The SBR mpu6050 installed just above the DC Motor.
 */
mpu6050_t hmpu;

/********************* end SINGLETON **************************/

#endif /* INC_MPU6050_H_ */
