/*
 * GccApplication3.c
 *
 * Created: 19.10.2023 10:01:11
 * Author : larskrud
 */ 


#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include "sam.h"
#include "delay.h"
#include "uart.h"
#include "printf-stdarg.h"
#include "ADC.h"
#include "pwm.h"
#include "motorcontrol.h"
#include "shooter.h"
#include "PID.h"
#include "game.h"
#include "can_controller.h"




uint32_t hexer(void){
	const uint32_t baud_rate = 125000UL;
	const uint8_t sync = 1;
	const uint8_t prop = 2;
	const uint8_t ps1 = 6;
	const uint8_t ps2 = 7;
	const uint8_t sjw = 3;
	
	const uint8_t nominal_can_bit_time = sync + prop + ps1 + ps2;
	
	const uint8_t brp = 84000000UL/(nominal_can_bit_time*baud_rate);

	const uint32_t can_hex_def = ((brp-1) << 16 | (sjw - 1) << 12 | (prop-1) << 8 | (ps1 - 1) << 4 | (ps2 - 1));
	return can_hex_def;
	
}



int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;        // Disable Watchdog Timer
	PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN); // PMC enable
	configure_uart();
	pwm_init();
	adc_init();
	motor_init();
	solenoid_init();
	
	can_init_def_tx_rx_mb(hexer());
	
	pidderer(10,0.005,0,1); //PID Parameters, Kp, Ki, Kd, T
	
	

    while (1) 
    {		
		counter();
			
    }
}
