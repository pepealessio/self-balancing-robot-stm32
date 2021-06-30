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
#include "mpu6050_k.h"
#include "usart.h"

/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/
#define RAD_TO_DEG 		(57.295779513082320876798154814105)

#define WHO_AM_I_REG 		(0x75)
#define PWR_MGMT_1_REG 		(0x6B)
#define SMPLRT_DIV_REG 		(0x19)
#define ACCEL_CONFIG_REG 	(0x1C)
#define ACCEL_XOUT_H_REG 	(0x3B)
#define TEMP_OUT_H_REG 		(0x41)
#define GYRO_CONFIG_REG 	(0x1B)
#define GYRO_XOUT_H_REG 	(0x43)

// Setup MPU6050
#define MPU6050_ADDR 		(0xD0)
const uint16_t i2c_timeout = 100;
const double Accel_Z_corrector = 14418.0;

/********************* end DEFINE **************************/

/********************* MPU VARIABLE ******************************/
uint32_t timer;

Kalman_t KalmanX = {
        .Q_angle = 0.001f,
        .Q_bias = 0.003f,
        .R_measure = 0.03f
};

Kalman_t KalmanY = {
        .Q_angle = 0.001f,
        .Q_bias = 0.003f,
        .R_measure = 0.03f,
};

/********************* end MPU VARIABLE **************************/

/********************* FUNCTION ******************************/
uint8_t MPU6050__Init(MPU6050_t *hmpu6050, I2C_HandleTypeDef *hi2c) {
    uint8_t check;
    uint8_t Data;

    hmpu6050->hi2c = hi2c;

    // check device ID WHO_AM_I
    HAL_I2C_Mem_Read(hmpu6050->hi2c, MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1, i2c_timeout);

    if (check == 104)  // 0x68 will be returned by the sensor if everything goes well
    {
        // power management register 0X6B we should write all 0's to wake the sensor up
        Data = 0;
        HAL_I2C_Mem_Write(hmpu6050->hi2c, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1, i2c_timeout);

        // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
        Data = 0x07;
        HAL_I2C_Mem_Write(hmpu6050->hi2c, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, i2c_timeout);

        // Set accelerometer configuration in ACCEL_CONFIG Register
        // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
        Data = 0x00;
        HAL_I2C_Mem_Write(hmpu6050->hi2c, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, i2c_timeout);

        // Set Gyroscopic configuration in GYRO_CONFIG Register
        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
        Data = 0x00;
        HAL_I2C_Mem_Write(hmpu6050->hi2c, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, i2c_timeout);
        return 0;
    }
    return 1;
}

uint8_t MPU6050__Read_All(MPU6050_t *hmpu6050) {
    uint8_t Rec_Data[14];
    int16_t temp;

    // Read 14 BYTES of data starting from ACCEL_XOUT_H register

    uint8_t status = HAL_I2C_Mem_Read(hmpu6050->hi2c, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 14, i2c_timeout);

    hmpu6050->Accel_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    hmpu6050->Accel_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
    hmpu6050->Accel_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);
    temp = (int16_t) (Rec_Data[6] << 8 | Rec_Data[7]);
    hmpu6050->Gyro_X_RAW = (int16_t) (Rec_Data[8] << 8 | Rec_Data[9]);
    hmpu6050->Gyro_Y_RAW = (int16_t) (Rec_Data[10] << 8 | Rec_Data[11]);
    hmpu6050->Gyro_Z_RAW = (int16_t) (Rec_Data[12] << 8 | Rec_Data[13]);

    hmpu6050->Ax = hmpu6050->Accel_X_RAW / 16384.0;
    hmpu6050->Ay = hmpu6050->Accel_Y_RAW / 16384.0;
    hmpu6050->Az = hmpu6050->Accel_Z_RAW / Accel_Z_corrector;
    hmpu6050->Temperature = (float) ((int16_t) temp / (float) 340.0 + (float) 36.53);
    hmpu6050->Gx = hmpu6050->Gyro_X_RAW / 131.0;
    hmpu6050->Gy = hmpu6050->Gyro_Y_RAW / 131.0;
    hmpu6050->Gz = hmpu6050->Gyro_Z_RAW / 131.0;

    // Kalman angle solve
    double dt = (double) (HAL_GetTick() - timer) / 1000;
    timer = HAL_GetTick();
    double roll;
    double roll_sqrt = sqrt(
    		hmpu6050->Accel_X_RAW * hmpu6050->Accel_X_RAW + hmpu6050->Accel_Z_RAW * hmpu6050->Accel_Z_RAW);
    if (roll_sqrt != 0.0) {
        roll = atan(hmpu6050->Accel_Y_RAW / roll_sqrt) * RAD_TO_DEG;
    } else {
        roll = 0.0;
    }
    double pitch = atan2(-hmpu6050->Accel_X_RAW, hmpu6050->Accel_Z_RAW) * RAD_TO_DEG;
    if ((pitch < -90 && hmpu6050->KalmanAngleY > 90) || (pitch > 90 && hmpu6050->KalmanAngleY < -90)) {
        KalmanY.angle = pitch;
        hmpu6050->KalmanAngleY = pitch;
    } else {
    	hmpu6050->KalmanAngleY = __Kalman_getAngle(&KalmanY, pitch, hmpu6050->Gy, dt);
    }
    if (fabs(hmpu6050->KalmanAngleY) > 90)
    	hmpu6050->Gx = -hmpu6050->Gx;
    hmpu6050->KalmanAngleX = __Kalman_getAngle(&KalmanX, roll, hmpu6050->Gy, dt);

    return status;
}

double __Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt) {
    double rate = newRate - Kalman->bias;
    Kalman->angle += dt * rate;

    Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
    Kalman->P[0][1] -= dt * Kalman->P[1][1];
    Kalman->P[1][0] -= dt * Kalman->P[1][1];
    Kalman->P[1][1] += Kalman->Q_bias * dt;

    double S = Kalman->P[0][0] + Kalman->R_measure;
    double K[2];
    K[0] = Kalman->P[0][0] / S;
    K[1] = Kalman->P[1][0] / S;

    double y = newAngle - Kalman->angle;
    Kalman->angle += K[0] * y;
    Kalman->bias += K[1] * y;

    double P00_temp = Kalman->P[0][0];
    double P01_temp = Kalman->P[0][1];

    Kalman->P[0][0] -= K[0] * P00_temp;
    Kalman->P[0][1] -= K[0] * P01_temp;
    Kalman->P[1][0] -= K[1] * P00_temp;
    Kalman->P[1][1] -= K[1] * P01_temp;

    return Kalman->angle;
};
/********************* end FUNCTION ******************************/

