

#ifndef INC_HBRIDGE_H_
#define INC_HBRIDGE_H_

/********************* INCLUDE ******************************/
#include "tim.h"
#include <stdint.h>
/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/
/********************* end DEFINE **************************/

/********************* TYPEDEF ******************************/

/**
 * The Hbridge structure is used to control the SBL's DC Motors,
 * using an STM32 timer in PWM mode.
 *
 * @attention	PLEASE, NOT refer directly on the attributes.
 */
typedef struct hbridge_s
{
	TIM_HandleTypeDef *htim;
} hbridge_t;

/********************* end TYPEDEF **************************/

/********************* PROTOTYPE ******************************/

/**
 * Initialize the PID controller with the standard parameters.
 *
 * @parameter hhbridge:		The hbridge handler's pointer.
 * @parameter htim:			A timer with 4 channel setted in PWM mode
 * 							(50 Hz).
 */
void hbridge__init(hbridge_t *hhbridge, TIM_HandleTypeDef *htim);

/**
 * Set the left and right motor power.
 *
 * @parameter hhbridge:	The hbridge handler's pointer.
 * @parameter powerl:	The power of the left motor (a value between
 * 						- tim.ARR and + tim.ARR)
 * @parameter powerr:	The power of the right motor (a value between
 * 						- tim.ARR and + tim.ARR)
 */
void hbridge__set_motor(hbridge_t *hhbridge, int64_t powerl, int64_t powerr);

/**
 * Private method to constraint a value in a module.
 */
int64_t __hbridge__costraint(hbridge_t *hhbridge, int64_t value);

/**
 * Private method to set value in the timer.
 */
void __hbridge__set_motor_l(hbridge_t *hhbridge, int64_t pwm);

/**
 * Private method to set value in the timer.
 */
void __hbridge__set_motor_r(hbridge_t *hhbridge, int64_t pwm);

/********************* end PROTOTYPE **************************/

/********************* SINGLETON ******************************/

/**
 * The motor controller installed on the SBL.
 */
hbridge_t hhbridge;

/********************* end SINGLETON **************************/

#endif /* INC_HBRIDGE_H_ */
