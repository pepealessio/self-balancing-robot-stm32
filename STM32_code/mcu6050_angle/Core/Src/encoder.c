#include "encoder.h"

void Encoder__init(Encoder_t *hencoder, TIM_HandleTypeDef *htim)
{
	hencoder->htim = htim;
	hencoder->prev_cnt = 0;
	hencoder->cnt = 0;
	hencoder->diff = 0;
	hencoder->dir = 0;

	HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);

	hencoder->prev_cnt = __HAL_TIM_GET_COUNTER(hencoder->htim);
}

void Encoder__read(Encoder_t *hencoder)
{
	hencoder->cnt = __HAL_TIM_GET_COUNTER(hencoder->htim);
	hencoder->dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(hencoder->htim);
	if (hencoder->dir)
	{
		if (hencoder->cnt <= hencoder->prev_cnt)
		{
			hencoder->diff = hencoder->prev_cnt - hencoder->cnt;
		}
		else
		{
			hencoder->diff = (__HAL_TIM_GET_AUTORELOAD(hencoder->htim) - hencoder->cnt) + hencoder->prev_cnt;
		}
	}
	else
	{
		if (hencoder->cnt >= hencoder->prev_cnt)
		{
			hencoder->diff = hencoder->cnt - hencoder->prev_cnt;
		}
		else
		{
			hencoder->diff = - ((__HAL_TIM_GET_AUTORELOAD(hencoder->htim) - hencoder->prev_cnt) + hencoder->cnt);
		}
	}
	hencoder->prev_cnt = __HAL_TIM_GET_COUNTER(hencoder->htim);
}
