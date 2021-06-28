
#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "tim.h"
#include <stdint.h>

typedef struct Encoder_s
{
	TIM_HandleTypeDef *htim;
	uint16_t prev_cnt;
	uint16_t cnt;
	int32_t diff;
	uint8_t dir;
} Encoder_t;


void Encoder__init(Encoder_t *hencoder, TIM_HandleTypeDef *htim);

void Encoder__read(Encoder_t *hencoder);


Encoder_t hencoderl;
Encoder_t hencoderr;

#endif /* INC_ENCODER_H_ */
