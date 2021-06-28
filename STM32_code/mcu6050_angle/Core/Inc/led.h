
#ifndef __LED_T
#define __LED_T

/********************* INCLUDE ******************************/
#include <stdbool.h>
/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/
/********************* end DEFINE **************************/

/********************* TYPEDEF ******************************/

/**
  * @brief  Structure that describes a simple external LED memorizing the
  *         necessary attributes to use it. 
  *        
  * @attention  Please <b> DON'T </b> reference directly the attributes, 
  *             use the init function "Led__init" or a getter method "Led__GPIOx", 
  *             "Led__GPIO_PIN".
  */
typedef struct Led_s
{
  GPIO_TypeDef *GPIOx; /* The GPIO port (e.g., GPIOA, GPIOB, ...) */
  uint16_t GPIO_PIN; /* The pin number (e.g., GPIO_PIN_1, ...) */

  bool locked_state;
} Led_t;

/********************* end TYPEDEF **************************/

/********************* PROTOTYPE ******************************/

/**
  * @brief  Inizialize the led state. 
  *
  * @param  self:     The pointer at a Led_t type 
  * @param  GPIOx:    The pointer to GPIO's port which the pin is connected.
  * @param  GPIO_PIN: The GPIO's pin number which the led is connected.
  */
void Led__init(Led_t *self, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN);

/**
  * @brief  Turn on the pin.
  *
  * @param  self: The pointer's structure that represents the pin to work on.
  */
void Led__turn_on(Led_t *self);

/**
  * @brief  Turn off the pin.
  *
  * @param  self: The pointer's structure that represents the pin to work on.
  */
void Led__turn_off(Led_t *self);

/**
  * @brief  Change the status of the LED (if it is on it turns it off, if it is
  *         off it turns it on).
  *
  * @param  self: The pointer's structure that represents the pin to work on.
  */
void Led__toggle(Led_t *self);

/**
  * @brief  Lock the led: Led__turn_on, Led__turn_off, and Led__toggle does not 
  * have any effect until Led__unlock is used.
  *
  * @param  self: The pointer's structure that represents the pin to work on.
  */
void Led__lock(Led_t *self);

/**
  * @brief  Unlock the led: Led__turn_on, Led__turn_off, and Led__toggle does  
  * have effect until Led__lock is used.
  *
  * @param  self: The pointer's structure that represents the pin to work on.
  */
void Led__unlock(Led_t *self);

/**
  * @brief  Getter methods for led status (locked or not).
  *
  * @param  self: The structure pointer's that represents the pin to work on.
  *
  * @retval A boolean value: true if the led is locked, false otherwise.
  */
bool Led__is_locked(Led_t *self);

/**
  * @brief  Getter methods for led GPIO port.
  *
  * @param  self: The structure pointer's that represents the pin to work on.
  *
  * @retval A GPIO_TypeDef pointer represents the port at witch the led is
  *         connected.
  */
GPIO_TypeDef *Led__GPIOx(Led_t *self);

/**
  * @brief  Getter methods for led pin number (e.g., GPIO_PIN_2).
  *
  * @param  self: The pointer's structure that represents the pin to work on.
  *
  * @retval A uint16_t value represents the pin at witch the led is connected.
  */
uint16_t Led__GPIO_PIN(Led_t *self);

/********************* end PROTOTYPE **************************/

/********************* SINGLETON ******************************/

/**
 * The led used to handle error.
 */
Led_t hled0;

/********************* end SINGLETON **************************/

#endif /* __LED_T */
