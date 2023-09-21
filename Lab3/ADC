#include <avr/io.h>
#define F_CPU 4915200// Clock Speed
#include <util/delay.h>


#define BASE_ADDRESS_ADC 0x1400
#define NUMBER_OF_CHANNELS 4

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include "uart.h"
#include "joysticking.h"

#define deadzone 0x08



void SRAM_init(){
	MCUCR = (1 << SRE);
	SFIOR = (1 << XMM2);
}

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
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
	uint8_t data;

	for (int i = 0; i <= channel; ++i) {
		data = ext_mem[0];
	}
	
	return data;
}



int main(void)
{
	UART_Init(MYUBRR);
	//SRAM_init();
	//SRAM_test();
	pwm_init();
	adc_init();
	uint8_t cal[4];
	calibrate(cal);
	
	DDRB &= ~_BV(PB0);
	DDRB &= ~_BV(PB1);
	
	uint8_t joyarr[2];
	uint8_t slidarr[2];	
	
	char dir;

	
	printf("Initialized \r\n");
	
	while(1){
		
		if(bit_is_set(PINB, PB0)){
			printf("RIGHT KNAPP TRYKKET\r\n");
		}
		if(bit_is_set(PINB, PB1)){
			printf("VENSTRE KNAPP TRYKKET\r\n");
		}
		
		joysticks(joyarr);
		direction(cal);
		_delay_ms(200);
		
		
		/*printf("\n\r X_AXIS:");
		joysticks(joyarr);
		printf("%d", joyarr[0]);
		printf("%% - - Y_AXIS:");
		joysticks(joyarr);
		printf("%d", joyarr[1]);
		printf("%% \r\n");
		*/
		printf("\n\r RIGHT SLIDER:");
		sliders(slidarr);
		printf("%d", slidarr[0]);
		printf(" - - LEFT SLIDER:");
		sliders(slidarr);
		printf("%d", slidarr[1]);
		printf("\r\n");
		
		_delay_ms(1000);
	}
	
}
