/**
  ******************************************************************************
  * @file           : mpu6050.c
  * @brief          : mpu6050 driver implementation restricted on principal
  * 				  function and for a SBR.
  * @version		: v1.0
  ******************************************************************************
  */

#ifndef INC_GY521_H_
#define INC_GY521_H_

/********************* INCLUDE ******************************/
#include <stdint.h>
#include "i2c.h"
/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/
/********************* end DEFINE **************************/

/********************* TYPEDEF ******************************/

/**
 * This structure contains all the result read from the mpu6050
 * and the evaluated value. Contains also the actual angle evaluated
 * as Kalman angle.
 *
 * @note	You can directly read value from the following variables,
 * 			without getter methods.
 *
 * @attention:	PLEASE NOT WRITE on the following variables. Use just
 * 				in read mode.
 */
typedef struct {
	I2C_HandleTypeDef *hi2c;

    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
    double Gx;
    double Gy;
    double Gz;

    float Temperature;

    double KalmanAngleX;
    double KalmanAngleY;
} MPU6050_t;

/**
 * This structure represent the math model describet in the Neto's Researh
 * "Kalman Filter-Based Yaw Angle Estimation by Fusing Inertial and
 * Magnetic Sensing".
 *
 * @attention:	PLEASE not refer this struct.
 */
typedef struct {
    double Q_angle;
    double Q_bias;
    double R_measure;
    double angle;
    double bias;
    double P[2][2];
} Kalman_t;

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
 * @return	the device's status: 0 if the device is connected and
 * 			work, 1 if an error occur.
 */
uint8_t MPU6050__Init(MPU6050_t *hmpu6050, I2C_HandleTypeDef *I2Cx);

/**
 * This function real all data (in locking mode) and save that in
 * the mpu6050_t attributes. Furthermore, compute the legible value
 * and the two angles.
 *
 * @param hmpu6050:	the handler's pointer of mpu6050 to read and control.
 */
uint8_t MPU6050__Read_All(MPU6050_t *hmpu6050);

/**
 * This method is used in the public API to compute the Kalman angle.
 *
 * @attention:	PLEASE NOT USE THAT EXPLICITELY, you can obtain some
 * 				insignificant values
 */
double __Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt);

/********************* end PROTOTYPE **************************/

/********************* SINGLETON ******************************/

/**
 * The SBR mpu6050 installed just above the DC Motor.
 */
MPU6050_t hmpu;

/********************* end SINGLETON **************************/

#endif /* INC_GY521_H_ */
