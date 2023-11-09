/*
 * PID.h
 *
 * Created: 07.11.2023 16:08:12
 *  Author: larskrud
 */ 


#ifndef PID_H_
#define PID_H_

typedef struct {
	float K_p;
	float K_i;
	float K_d;
	int Integrator_error;
	int Previous_error;
	float Sample_period;
} PID_Parameters;


void pidderer(float k_p, float k_i, float k_d, float Sample_period);
	
void pid_reset_errors();
	
int pid(int ref, int joystick_value);





#endif /* PID_H_ */