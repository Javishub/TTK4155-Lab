/*
 * game.c
 *
 * Created: 09.11.2023 10:38:21
 *  Author: larskrud
 */ 

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include "sam.h"
#include "can_controller.h"

uint32_t mscore = 0;
uint16_t score = 0;
uint16_t previous_score = 0;

CAN_MESSAGE score_msg = {
	1,
	7,
	{0,0,0,0,0,0,0,0}
	};


uint16_t counter(){
	SysTick->LOAD = 0x2904;
	SysTick->CTRL = 0x1;
	if(SysTick->CTRL & (1<<16)){
		mscore++;
	}
	if(mscore >= 1000){
		score++;
		mscore = 0;
	}
	if (previous_score != score)
	{
		previous_score = score;
		score_msg.data[0] = score;
		score_msg.data[1] = 0;
		can_send(&score_msg, 0);
		//printf("Send val: %d", can_send(&score_msg, 0));
		printf("Score: %d\n\r", score_msg.data[0]);
		//printf("SCORE: %d\n\r", score);
	}
}

void reset_score(){
	score_msg.data[0] = score;
	score_msg.data[1] = 255;
	can_send(&score_msg, 0);
	score = 0;
	mscore = 0;
}

uint16_t scoring(int divider){
	mscore++;	
	if(mscore >= divider){
		score++;
		mscore = 0;
	}
	if (previous_score != score)
	{
		previous_score = score;
		score_msg.data[0] = score;
		score_msg.data[1] = 0;
		can_send(&score_msg, 0);
		//printf("Send val: %d", can_send(&score_msg, 0));
		printf("Score: %d\n\r", score_msg.data[0]);
		//printf("SCORE: %d\n\r", score);
	}
}
