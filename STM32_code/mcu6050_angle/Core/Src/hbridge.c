
/********************* INCLUDE ******************************/
#include "hbridge.h"
/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/
/********************* end DEFINE **************************/

/********************* FUNCTION ******************************/
void hbridge__init(hbridge_t *hhbridge, TIM_HandleTypeDef *htim)
{
	// Use one timer with 4 pwm channel.
	hhbridge->htim = htim;
	// Start the 4 channel with PWM.
	// We use first 2 channel for left motor power and direction
	HAL_TIM_PWM_Start(hhbridge->htim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(hhbridge->htim, TIM_CHANNEL_2);
	// We use last 2 channel for left motor power and direction
	HAL_TIM_PWM_Start(hhbridge->htim, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(hhbridge->htim, TIM_CHANNEL_4);
}

void hbridge__set_motor(hbridge_t *hhbridge, int64_t powerl, int64_t powerr)
{
	// Constrain power into [-TIM1->ARR, TIM1->ARR]
	powerl = __hbridge__costraint(hhbridge, powerl);
	powerr = __hbridge__costraint(hhbridge, powerr);

	// Now set the power.
	__hbridge__set_motor_l(hhbridge, powerl);
	__hbridge__set_motor_r(hhbridge, powerr);

}

int64_t __hbridge__costraint(hbridge_t *hhbridge, int64_t value)
{
	if (value < - (int64_t) hhbridge->htim->Instance->ARR)
	{
		value = - (int64_t) hhbridge->htim->Instance->ARR;
	}
	else if (value > (int64_t) hhbridge->htim->Instance->ARR)
	{
		value = (int64_t) hhbridge->htim->Instance->ARR;
	}
	return value;
}

void __hbridge__set_motor_l(hbridge_t *hhbridge, int64_t pwm)
{
	if (pwm >= 0)
	{
		TIM1->CCR1 = pwm;
		TIM1->CCR2 = 0;
	}
	else
	{
		TIM1->CCR1 = 0;
		TIM1->CCR2 = (uint32_t) - pwm;
	}
}

void __hbridge__set_motor_r(hbridge_t *hhbridge, int64_t pwm)
{
if (pwm >= 0)
	{
		TIM1->CCR3 = pwm;
		TIM1->CCR4 = 0;
	}
	else
	{
		TIM1->CCR3 = 0;
		TIM1->CCR4 = (uint32_t) - pwm;
	}
}
/********************* end FUNCTION ******************************/
