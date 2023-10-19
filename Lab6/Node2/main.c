/*
 * GccApplication3.c
 *
 * Created: 19.10.2023 10:01:11
 * Author : larskrud
 */ 


// BRUK 8752 som BAUDRATE VED KOMMUNIKASJON PÅ DUE
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "sam.h"
#include "uart.h"
#include "printf-stdarg.h"
#include "can_controller.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;        // Disable Watchdog Timer
	PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN); // PMC enable
	configure_uart();
	
	
	//uint8_t BRP = MCP_CPU / (2*NUMBER_OF_TQ*BAUDRATE)
	can_init_def_tx_rx_mb(0x00153355);
	
	
	CAN_MESSAGE msg;
	

    /* Replace with your application code */
    while (1) 
    {
		uint8_t status = can_receive(&msg, 0x00);
		printf("%d\n\r", status);
		
		for (int i = 0; i<8; i++)
		{
			printf("letter: %c", msg.data[i]);
		}
		if (!status)
		{
			// message handling
		}
    }
}
