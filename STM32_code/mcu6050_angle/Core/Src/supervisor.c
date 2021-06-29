
/********************* INCLUDE ******************************/
#include "i2c.h"
#include "supervisor.h"
#include "pid_controller.h"
/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/
/********************* end DEFINE **************************/

/********************* FUNCTION ******************************/
void FSM__init(FSM__current_t *hfsm)
{
	hfsm->vh = 0;
	hfsm->current_control_message = 0x00;
	hfsm->message_changed = true;
	hfsm->left_target = 0;
	hfsm->right_target = 0;
	hfsm->angle_offset = FSM__ANGLE_OFFSET;
}

void FSM__set_control_message(FSM__current_t *hfsm, FSM__control_message new_message)
{
	hfsm->current_control_message = new_message;
	hfsm->message_changed = true;
}

void FSM__update(FSM__current_t *hfsm)
{
	int64_t v;

	if (hfsm->message_changed)
	{
		/* Evaluate vh just one time (to reduce update time) */
		hfsm->vh = (hfsm->current_control_message & FSM__vh_mask) + 1;

		// Evaluate speed velocity change
		//v = FSM__MAX_VELOCITY * hfsm->vh / 8;
		v = FSM__MAX_VELOCITY;

		// Reset actual target
		hfsm->left_target = 0;
		hfsm->right_target = 0;
		hfsm->angle_offset = FSM__ANGLE_OFFSET;

		// Compute new target
		if (!(hfsm->current_control_message & FSM__up_mask))
		{
			hfsm->left_target += v;
			hfsm->right_target += v;
			hfsm->angle_offset += FSM__ANGLE_VARIATION;
		}
		if (!(hfsm->current_control_message & FSM__down_mask))
		{
			hfsm->left_target -= v;
			hfsm->right_target -= v;
			hfsm->angle_offset -= FSM__ANGLE_VARIATION;
		}
		if (!(hfsm->current_control_message & FSM__left_mask))
		{
			hfsm->left_target -= v;
			hfsm->right_target += v;
		}
		if (!(hfsm->current_control_message & FSM__right_mask))
		{
			hfsm->left_target += v;
			hfsm->right_target -= v;
		}

		/* Not re_evaluate until the next control message will come. */
		hfsm->message_changed = false;
	}
}
/********************* end FUNCTION ******************************/
