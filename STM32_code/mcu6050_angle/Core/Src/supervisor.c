
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
	hfsm->current_control_message = 0x00;
	hfsm->message_changed = true;
	hfsm->left_target = 0;
	hfsm->right_target = 0;
}

void FSM__set_control_message(FSM__current_t *hfsm, FSM__control_message new_message)
{
	hfsm->current_control_message = new_message;
	hfsm->message_changed = true;
}

void FSM__update(FSM__current_t *hfsm)
{
	if (hfsm->message_changed)
	{
		// Reset actual target
		hfsm->left_target = 0;
		hfsm->right_target = 0;
		pid__set_target(&hangle_pid, FSM__ANGLE_OFFSET);
		// Compute new target
		if (!(hfsm->current_control_message & FSM__up_mask))
		{
			hfsm->left_target += FSM__MAX_VELOCITY;
			hfsm->right_target += FSM__MAX_VELOCITY;
			pid__set_target(&hangle_pid, pid__get_target(&hangle_pid) + FSM__ANGLE_VARIATION);
		}
		if (!(hfsm->current_control_message & FSM__down_mask))
		{
			hfsm->left_target -= FSM__MAX_VELOCITY;
			hfsm->right_target -= FSM__MAX_VELOCITY;
			pid__set_target(&hangle_pid, pid__get_target(&hangle_pid) - FSM__ANGLE_VARIATION);
		}
		if (!(hfsm->current_control_message & FSM__left_mask))
		{
			hfsm->left_target -= FSM__MAX_VELOCITY;
			hfsm->right_target += FSM__MAX_VELOCITY;
		}
		if (!(hfsm->current_control_message & FSM__right_mask))
		{
			hfsm->left_target += FSM__MAX_VELOCITY;
			hfsm->right_target -= FSM__MAX_VELOCITY;
		}

		/* Not re_evaluate until the next control message will come. */
		hfsm->message_changed = false;
	}
}
/********************* end FUNCTION ******************************/
