
#ifndef INC_PID_CONTROLLER_H_
#define INC_PID_CONTROLLER_H_

/********************* INCLUDE ******************************/
#include <math.h>
#include <stdint.h>
/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/

/**
 * The interval time between the PID evaluate the control output.
 */
#define PID_CONTROLLER__DT		(15) /* ms */

/********************* end DEFINE **************************/

/********************* TYPEDEF ******************************/

/**
 * The PID structure used to memorize parameter, target and useful
 * variable used in the computation.
 *
 * @attention	PLEASE, NOT refer directly on the attributes. Use
 * 				the function described below (in pid_controller.h).
 */
typedef struct pid_controller_s
{
	// PID Parameters
	int32_t kp;
	int32_t ki;
	int32_t kd;
	// Value used to set the target
	int32_t target;
	// Value used to evaluate the I part
	int32_t error_sum;
	// Value used to compute D control part
	int32_t last_read_value;
} pid_controller_t;

/********************* end TYPEDEF **************************/

/********************* PROTOTYPE ******************************/

/**
 * Initialize the PID controller with the standard parameters.
 *
 * @parameter hpid:		The PID handler's pointer.
 * @parameter kp:		The Kp proportional constant.
 * @parameter ki:		The Ki integrative constant.
 * @parameter kd:		The Kd derivative constant.
 * @parameter target:	The initial target of the PID.
 *
 * @note:	Ki is not proportional at Kd and Kp. Ki have 100 level
 * 			more of precision.
 */
void pid__init(pid_controller_t *hpid, int32_t kp, int32_t ki, int32_t kd, int32_t target);

/**
 * Change the target who PID is following.
 *
 * @parameter hpid:		The PID handler's pointer.
 * @parameter target:	The new target.
 */
void pid__set_target(pid_controller_t *hpid, int32_t target);

/**
 * Getter method to read the current target value.
 *
 * @parameter hpid:		The PID handler's pointer.
 *
 * @return	The current target value.
 */
int32_t pid__get_target(pid_controller_t *hpid);

/**
 * The PID receive the current read value and compute the new current output.
 *
 * @parameter hpid:			The PID handler's pointer.
 * @parameter read_value:	The current value of the variable to control.
 *
 * @return	The new PID current output.
 */
int64_t pid__evaluate_output(pid_controller_t *hpid, int64_t read_value);

int32_t __pid_costraint(int32_t value, int32_t max_abs_value);

/********************* end PROTOTYPE **************************/

/********************* SINGLETON ******************************/

/**
 * The PID used to evaluate the PWM output offset respect to the
 * equilibrium (angle) of the SBL.
 */
pid_controller_t hangle_pid;

/**
 * The PID used to control the left motor of the SBL respect the
 * speed target.
 */
pid_controller_t hmotorl_pid;

/**
 * The PID used to control the right motor of the SBL respect the
 * speed target.
 */
pid_controller_t hmotorr_pid;

/********************* end SINGLETON **************************/

#endif /* INC_PID_CONTROLLER_H_ */
