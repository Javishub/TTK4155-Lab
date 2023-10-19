void SRAM_init(){
	MCUCR = (1 << SRE);
	SFIOR = (1 << XMM2);
}


void adc_init() {
	MCUCR = (1 << SRE); // Enable SRAM i ATmega162
	SFIOR = (1 << XMM2); // Maskerer PC4-PC7 på ATmega162.
}


void pwm_init() {
	// set PD4 to output PWM timer signal
	DDRD |= (1<<PD4);

	// set to CTC mode
	TCCR3A |= (1 << WGM31);
	TCCR3A |= (1 << WGM30);
	
	TCCR3B |= (1 << WGM32);
	TCCR3B |= (1 << WGM33);
	
	TCCR3A &= ~(1 << COM3A1);
	TCCR3A |= (1 << COM3A0);
	
	OCR3A  = 1;

	// set PWM signal to match with internal clock, no prescale
	TCCR3B &= ~(1 << CS02);
	TCCR3B &= ~(1 << CS01);
	TCCR3B |= (1 << CS00);

}


uint8_t adc_read(uint8_t channel){
	// initiate conversion by setting writing to the ADC address space
	volatile char* ext_mem = (char *) BASE_ADDRESS_ADC; // ext_mem is a pointer for a variable that store memory adresses
	ext_mem[0] = 0;

	// wait for end of conversion
	_delay_ms(9*NUMBER_OF_CHANNELS*2 / F_CPU);

	// read desired channel
	uint8_t data = 0;

	for (int i = 0; i <= channel; ++i) {
		data = ext_mem[0];
	}
	
	return data;
}