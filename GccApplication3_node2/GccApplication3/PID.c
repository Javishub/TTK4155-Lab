/*
 * GccApplication10.c
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
int maxU = 0x4FF;



#include "sam.h"

void pidderer(float k_p, float k_i, float k_d, float Sample_period) {
	PID.K_p = k_p;
	PID.K_i = k_i;
	PID.K_d = k_d;
	PID.Sample_period = Sample_period;
}


void pid_reset_errors() {
	PID.Integrator_error = 0;
	PID.Previous_error = 0;
}


int pid(int ref, int encoder) { //Wanted pos, actual pos
	
	float error = ref - encoder;
	PID.Integrator_error += error;

	float u_p = PID.K_p * error;
	float u_i = PID.Sample_period * PID.K_i * PID.Integrator_error;
	float u_d = (PID.K_d / PID.Sample_period) * (error - PID.Previous_error);
	float u = (u_p+u_i+u_d);
	
	if (u > maxU) {
		u = maxU;
	}
	else if (u < -maxU) {
		u = -maxU;
	}



	PID.Previous_error = error;
	delay_ms(PID.Sample_period);
	
	return u;
}