/*
 * delay.h
 *
 * Created: 04.11.2023 13:19:58
 *  Author: larskrud
 */ 





#define IRQ_SysTick_priority    0


volatile uint32_t wait_ticks = 0;


static void SysTick_init_us(int period) {
	// set SysTick reload value
	SysTick->LOAD = ((int)(period*84) & SysTick_LOAD_RELOAD_Msk)-1;

	// reset SysTick counter value
	SysTick->VAL = 0;

	// set SysTick interrupt priority
	NVIC_SetPriority(SysTick_IRQn, IRQ_SysTick_priority);

	// set SysTick timer to MCK, enable interrupt and timer
	SysTick->CTRL  = (1 << SysTick_CTRL_CLKSOURCE_Pos) & SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= (1 << SysTick_CTRL_TICKINT_Pos) & SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos) & SysTick_CTRL_ENABLE_Msk;
}


void _delay_us(uint16_t us) {
	wait_ticks = us;
	SysTick_init_us(1);
	while(wait_ticks != 0);
}


void _delay_ms(uint16_t ms) {
	wait_ticks = ms;
	SysTick_init_us(1000);
	while(wait_ticks != 0);
}


void SysTick_Handler(void) {
	if(wait_ticks!=0) {
		wait_ticks--;
	}
	else {
		SysTick->CTRL = 0;
	}
}