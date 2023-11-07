/*
 * GccApplication10.cpp
 *
 * Created: 04.11.2023 13:23:40
 * Author : Ludvi
 */ 

typedef struct {
	float K_p;
	float K_i;
	float K_d;
	int Integrator_error;
	int Previous_error;
	float Sample_period;
} PID_Parameters;
PID_Parameters PID;


#include "sam.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();

    /* Replace with your application code */
    while (1) 
    {
    }
}


void pid_parameters(float k_p, float k_i, float k_d, float Sample_period) {
	PID.K_p = k_p;
	PID.K_i = k_i;
	PID.K_d = k_d;
	PID.Sample_period = Sample_period;
}


void pid_reset_errors() {
	PID.Integrator_error = 0;
	PID.Previous_error = 0;
}


int pid(int ref, int joystick_value) {
	int error = ref - joystick_value;
	PID.Integrator_error += error;

	int u_p = PID.K_p * error;
	int u_i = PID.Sample_period * PID.K_i * PID.Integrator_error;
	int u_d = (PID.K_d / PID.Sample_period) * (error - PID.Previous_error);
	int u = u_p + u_i + u_d;

	PID.Previous_error = error;
	

	return u;
}
