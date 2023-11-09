/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"

#include "printf-stdarg.h"

#include "can_controller.h"

#define DEBUG_INTERRUPT 0


/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */


int divider = 1000;

	
void CAN0_Handler( void )
{
	scoring(divider);
	int ir_level = 0;
	uint8_t dutycycle = 0;
	int mailboxerror = 0;
	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);
			mailboxerror = 0;

		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
			mailboxerror = 0;
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
			mailboxerror = 1;
		}

		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
		
		ir_level = adc_read();
		
		if(ir_level < 2000){
			reset_score();
		}
		
		if (!mailboxerror)
		{
			servo_pos(message.data[0]);			
			if(message.data[2]){
			solenoid_shoot(message.data[2]);
			divider = 500000;
			}
			/*if(message.data[1] < 180 & message.data[1] > 160){
				disable_motor();
			}*/
			
			enable_motor();
			joystick_motoring(message.data[1]);
			
			if(message.data[3]){
				reset_score();
			}
			
// 			printf("X akse: %d\n\r", message.data[0]);
// 			printf("Y akse: %d\n\r", message.data[1]);
			
			/*switch(message.data[0]){
				case 0:
				//printf("NEUTRAL\n\r");
				pwm_set_dutycycle(1.5);
				disable_motor();
				break;
				case 1:
				//printf("DOWN\n\r");
				enable_motor();
				joystick_motoring(1);
				break;
				case 2:
				//printf("UPWARDS\n\r");
				enable_motor();
				joystick_motoring(2);
				break;
				case 3:
				//printf("LEFT\n\r");
				pwm_set_dutycycle(2.1);
				break;
				case 4:
				//printf("RIGHT\n\r");
				pwm_set_dutycycle(0.9);
				break;
				default:
				break;
			
		
			}*/
			
		}
		


	}	

			
			
	
	if(can_sr & CAN_SR_MB0)
	{
		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}
