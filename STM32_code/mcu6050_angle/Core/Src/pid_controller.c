
/********************* INCLUDE ******************************/
#include "pid_controller.h"
/********************* end INCLUDE **************************/

/********************* DEFINE ******************************/
/********************* end DEFINE **************************/

/********************* FUNCTION ******************************/

void pid__init(pid_controller_t *hpid, int32_t kp, int32_t ki, int32_t kd, int32_t target)
{
	// Just set all initial value
	hpid->kp = kp;
	hpid->ki = ki;
	hpid->kd = kd;
	hpid->target = target;
	// set initial error_sum to 0
	hpid->error_sum = 0;
	// set initial last error to 0 error (so last read value to target value)
	hpid->last_read_value = target;
}

void pid__set_target(pid_controller_t *hpid, int32_t target)
{
	hpid->target = target;
}

int32_t pid__get_target(pid_controller_t *hpid)
{
	return hpid->target;
}

int64_t pid__evaluate_output(pid_controller_t *hpid, int64_t read_value)
{
	int32_t error, output;

	// compute the actual error
	error = read_value - hpid->target;
	// update the error sum used to evaluate error's integral
	hpid->error_sum += error;

	// evaluate PID output like $kp*error + \int_{0}^{t}{error dt} + \frac{d error}{dt}$"
	output = (hpid->kp * error) + (hpid->ki * hpid->error_sum * PID_CONTROLLER__DT / 100) + hpid->kd * ((read_value - hpid->target) / PID_CONTROLLER__DT);

	// update last read value used in derivative error
	hpid->last_read_value = read_value;

	return output;
}

/********************* end FUNCTION ******************************/

