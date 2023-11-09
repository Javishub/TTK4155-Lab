/*
 * GccApplication3.c
 *
 * Created: 19.10.2023 10:01:11
 * Author : larskrud
 */ 


// BRUK 8752 som BAUDRATE VED KOMMUNIKASJON P� DUE
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include "sam.h"
//#include "time.h"
#include "delay.h"
#include "uart.h"
#include "printf-stdarg.h"
#include "can_controller.h"
#include "pwm.h"
#include "ADC.h"
#include "motorcontrol.h"
#include "shooter.h"
#include "PID.h"
#include "game.h"
//#include "CAN_BAUD_CONF.h"
// can_binary_ting			0000000 1 0 0010101 00 11 0 010 0 110 0 111
// can_binary_ting_minus_en 0000000 1 0 0010100 00 11 0 001 0 101 0 110
#define can_hex_ting 0x01153267

static int butt = 0;



uint32_t hexer(void){
	const uint32_t baud_rate = 125000UL;
	const uint8_t sync = 1;
	const uint8_t prop = 2;
	const uint8_t ps1 = 6;
	const uint8_t ps2 = 7;
	const uint8_t sjw = 3;
	
	const uint8_t nominal_can_bit_time = sync + prop + ps1 + ps2;
	
	const
	
	const uint8_t brp = 84000000UL/(nominal_can_bit_time*baud_rate);
	//printf("BRP: %x", brp);


	const uint32_t can_hex_def = ((brp-1) << 16 | (sjw - 1) << 12 | (prop-1) << 8 | (ps1 - 1) << 4 | (ps2 - 1));
	return can_hex_def;
	
}

void buttonstate(int val){
	butt = val;
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
	
	//uint8_t BRP = MCP_CPU / (2*NUMBER_OF_TQ*BAUDRATE)
	//printf("DETTE ER VAAR HEX %x\r\n", hexer());
	
	can_init_def_tx_rx_mb(hexer());

	int score = 0;
	
	pidderer(10,0.005,0,1);
	
	int ir_level = 0;
	

    /* Replace with your application code */
    while (1) 
    {
		
		counter();
		
		
		
		
		//printf("irlevle: %d\n\r", ir_level);
		//looper++;
		//printf("looper: %d\n\r", looper);
// 		if(ir_level > 1700){
// 			reset_score();
// 		}
		//printf("Encoder: %d\n\r", read_motor_encoder());
// 		uint32_t tid = get_micros();
// 		solenoid_shoot(butt, tid);
// 		printf("TIDAER: %d \n\r", tid);
		
		
		//printf("Encoder: %x\n\r",read_motor_encoder());
		
			
    }
}
