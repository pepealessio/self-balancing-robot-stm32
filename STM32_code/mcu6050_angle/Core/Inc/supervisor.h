

#ifndef INC_SUPERVISOR_H_
#define INC_SUPERVISOR_H_

#include <stdbool.h>
#include <stdint.h>

/********************* DEFINE ******************************/

#define FSM__MAX_VELOCITY 		(110)
#define FSM__ANGLE_OFFSET   	(370) // W is -
#define FSM__ANGLE_VARIATION	(110)

// buz | up | down | left | right | v0 | v1 | v2

#define FSM__vh_mask		(0x07)

#define FSM__up_mask					(0x40)
#define FSM__down_mask					(0x20)
#define FSM__left_mask					(0x10)
#define FSM__right_mask					(0x08)

#define FSM__buz_mask					(0x80)

/********************* end DEFINE **************************/

/********************* TYPEDEF ******************************/

/**
 * The control message structured as below
 * ~buz | ~up | ~down | ~left | ~right | v0 | v1 | v2
 */
typedef uint8_t FSM__control_message;

typedef struct FSM__current_s
{
	uint8_t current_control_message;
	bool message_changed;
	uint8_t vh;
	int64_t left_target;
	int64_t right_target;
	int64_t angle_offset;
} FSM__current_t;

/********************* end TYPEDEF **************************/

/********************* PROTOTYPE ******************************/

/**
 * Use this function to set a new control message. That will be evaluated
 * in the next main cycle. If you use set multiple times in just one
 * cycle, just the newer message will be evaluated.
 */
void FSM__set_control_message(FSM__current_t *hfsm, FSM__control_message new_message);

/**
 * Set the initial state and the initial parameter.
 */
void FSM__init(FSM__current_t *hfsm);

/**
 * Evolve the FSM representing the system.
 */
void FSM__update(FSM__current_t *hfsm);

/********************* end PROTOTYPE **************************/

/********************* SINGLETON ******************************/

/**
 * The FSM used to supervised the SBR controller scheme via
 * BT message received.
 */
FSM__current_t hfsm;
uint8_t hfsm_buffer;

/********************* end SINGLETON **************************/

#endif /* INC_SUPERVISOR_H_ */
