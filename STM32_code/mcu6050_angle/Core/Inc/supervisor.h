

#ifndef INC_SUPERVISOR_H_
#define INC_SUPERVISOR_H_

/********************* INCLUDE ******************************/

#include <stdbool.h>
#include <stdint.h>

/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/


#define FSM__MAX_VELOCITY 		(10100)
#define FSM__ANGLE_OFFSET   	(-130) // W is +
#define FSM__ANGLE_VARIATION	(-105) // W is +

// The control byte is like the following:
// buz | up | down | left | right | res | res | res

#define FSM__buz_mask					(0x80)
#define FSM__up_mask					(0x40)
#define FSM__down_mask					(0x20)
#define FSM__left_mask					(0x10)
#define FSM__right_mask					(0x08)

/********************* end DEFINE **************************/

/********************* TYPEDEF ******************************/

/**
 * The control message structured as below
 * buz | up | down | left | right | res | res | res
 */
typedef uint8_t FSM__control_message;

typedef struct FSM__current_s
{
	FSM__control_message current_control_message;
	bool message_changed;
	int64_t left_target;
	int64_t right_target;
	int64_t angle_offset;
} FSM__current_t;

/********************* end TYPEDEF **************************/

/********************* PROTOTYPE ******************************/

/**
 * Set the initial state and the initial parameter.
 */
void FSM__init(FSM__current_t *hfsm);

/**
 * Use this function to set a new control message. That will be evaluated
 * in the next main cycle. If you use set multiple times in just one
 * cycle, just the newer message will be evaluated.
 */
void FSM__set_control_message(FSM__current_t *hfsm, FSM__control_message new_message);

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
