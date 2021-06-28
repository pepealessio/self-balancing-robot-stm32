
/********************* INCLUDE ******************************/
/* required STM32F401 library */
#include "gpio.h"
#include <stdbool.h>

/* Led_t header file */
#include "led.h"
/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/
/********************* end DEFINE **************************/

/********************* TYPEDEF ******************************/
/********************* end TYPEDEF **************************/

/********************* FUNCTION ******************************/

void Led__init(Led_t *self, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN)
{
  self->GPIOx = GPIOx;
  self->GPIO_PIN = GPIO_PIN;
  self->locked_state = false;
}

void Led__turn_on(Led_t *self)
{
  if(self && !self->locked_state)
  {
    HAL_GPIO_WritePin(self->GPIOx, self->GPIO_PIN, GPIO_PIN_SET);
  }
}

void Led__turn_off(Led_t *self)
{
  if(self && !self->locked_state)
  {
    HAL_GPIO_WritePin(self->GPIOx, self->GPIO_PIN, GPIO_PIN_RESET);
  }
}

void Led__toggle(Led_t *self)
{
  if(self && !self->locked_state)
  {
    HAL_GPIO_TogglePin(self->GPIOx, self->GPIO_PIN);
  }
}

void Led__lock(Led_t *self)
{
  if(self)
  {
    self->locked_state = true;
  }
}

void Led__unlock(Led_t *self)
{
  if(self)
  {
    self->locked_state = false;
  }
}

bool Led__is_locked(Led_t *self)
{
  if(self)
  {
    return self->locked_state;
  }
  return false;
}

GPIO_TypeDef *Led__get_port(Led_t *self)
{
  if(self)
  {
    return self->GPIOx;
  }
  return NULL;
}

uint16_t Led__get_pin(Led_t *self)
{
  if(self)
  {
    return self->GPIO_PIN;
  }
  return -1;
}

/********************* end FUNCTION **************************/

