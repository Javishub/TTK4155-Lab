/*
 * delay.h
 *
 * Created: 04.11.2023 13:19:58
 *  Author: larskrud
 */ 

void delay_ms(uint32_t delay){
	uint32_t ms = 0;
	SysTick->LOAD = 0x2904;
	SysTick->CTRL = 0x1;
	
	while(ms < delay){
		if(SysTick->CTRL & (1<<16)){
			ms++;
		}
	}
}
void delay_us(uint32_t delay){
	uint32_t us = 0;
	SysTick->LOAD = 0x2904/1000;
	SysTick->CTRL = 0x1;
	
	while(us < delay){
		if(SysTick->CTRL & (1<<16)){
			us++;
		}
	}
}