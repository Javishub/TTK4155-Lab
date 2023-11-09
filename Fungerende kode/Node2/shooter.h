/*
 * shooter.h
 *
 * Created: 04.11.2023 14:51:41
 *  Author: larskrud
 */ 



static int flag = 0;
uint32_t current_time = 0;
uint32_t previous_time = 0;
uint32_t ms = 0;
float wait_time = 80;			//80ms?

void solenoid_init(){
	PIOA->PIO_CODR |= PIO_PA16;
	PIOA->PIO_PER  |= PIO_PA16;
	PIOA->PIO_OER  |= PIO_PA16;
	PIOA->PIO_SODR |= !PIO_PA16;
}

void solenoid_shoot(int butt){
// 	current_time = get_micros();
// 	printf("This is the current time: %d\n\r", current_time);
	uint32_t debounce = 2;
	while (debounce > ms)
	{
		if(SysTick->CTRL & (1<<16)){
			ms++;
		}
		return;
	}
	ms = 0;
	if (butt == 1){
		
		PIOA->PIO_SODR |= PIO_PA16;
		delay_ms(80);
		PIOA->PIO_CODR |= PIO_PA16;
		
	}
	return;	
}		
		/*if (current_time-previous_time >= wait_time*1000){
			PIOA->PIO_CODR |= PIO_PA16;
			PIOA->PIO_SODR |= !PIO_PA16;
			printf("TIME AT SHOOTING: %d\n\r", current_time-previous_time);
			previous_time = current_time;
		}*/
		