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
#include "OLED.h"
#include "fonts.h"
#include "menus.h"
#include "SPI.h"

#define deadzone 0x08


typedef enum {highscore, play_game} currentState_t;



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
	oled_init();
	spi_master_init();
	uint8_t cal[4];
	calibrate(cal);
	oled_write_command(0x20);
	oled_write_command(0b00);
	oled_reset();
	oled_brightness(100); //Brightness of display from 0-100%
	
	printf("\r\nBEGIN------");
	
	DDRB &= ~_BV(PB0);
	DDRB &= ~_BV(PB1);
	
	uint8_t joyarr[2];
	uint8_t slidarr[2];	
	
	
	while(1){
	_delay_ms(10);
	spi_write('a');
	uint8_t readeded = spi_read();
	//printf("\r\n%x", readeded);
	}
	
	
	
	
	/*menu_ptr menu = malloc(sizeof(menu_t));
	
	menu_ptr menu_highscore = menu_add(menu, "Vis Highscore", &show_highscore);
	menu_ptr menu_play = menu_add(menu, "New Game", &playgame);
	
	menu_ptr menu_game1 = menu_add(menu_play, "Save 1", &play_game1);
	menu_ptr menu_game2 = menu_add(menu_play, "Save 2", &play_game2);
	
	
	int arrow_pos=0;
	int acc=0;
	directions_t currentDir = neutral;
	directions_t previousDir = neutral;
	printf("\r\n--------------------\n");
	
	while(1){
		
	directions_t dir = direction(cal);
	
	if (dir != previousDir) {
            // Joystick state has changed
            previousDir = currentDir;

            // Handle state transitions
            switch (dir) {
                case down:
                    currentDir = down;
                    break;
                case up:
                    currentDir = up;
                    break;
                case left:
                    currentDir = left;
                    break;
                case right:
                    currentDir = right;
                    break;
                case neutral:
                    // Joystick is back to center, but we retain the previous state.
                    break;
                default:
                    break;
            }
        }
	
	
	if (arrow_pos == 0)
	{
		oled_pos(arrow_pos,0);
		oled_arrow();
		oled_print(menu_highscore->text);
		oled_clear_line(arrow_pos+2);
		oled_pos(arrow_pos+2,5);
		oled_print(menu_play->text);
	}else if (arrow_pos == 2)
	{
		oled_clear_line(arrow_pos-2);
		oled_pos(arrow_pos-2,5);
		oled_print(menu_highscore->text);
		oled_pos(arrow_pos,0);
		oled_arrow();
		oled_print(menu_play->text);
	}	
	
	
	if (currentDir == down && arrow_pos == 0)
	{
		arrow_pos=2;
	}
	if (currentDir == up && arrow_pos == 2)
	{
		arrow_pos=0;
	}
	
	if (bit_is_set(PINB, PB1) && arrow_pos == 0){
		acc = 1;
 		printf("\r\nHighscore selected");
		oled_reset();
	
	}
	if(bit_is_set(PINB, PB1) && arrow_pos == 2){
 		printf("\r\nPlay selected");
		acc = 2;
		oled_reset();
	}
	
	while (acc == 1)
	{
		oled_pos(0,0);
		menu_highscore->function();
		
		if (bit_is_set(PINB, PB0)==1)
		{
			acc = 0;
			oled_reset();
		}
	}
	while (acc == 2){
		oled_pos(0,0);
		dir = direction(cal);
	
		if (dir != previousDir) {
            // Joystick state has changed
            previousDir = currentDir;

            // Handle state transitions
            switch (dir) {
                case down:
                    currentDir = down;
                    printf("\r\nJoystick is down.\n");
                    break;
                case up:
                    currentDir = up;
                    printf("\r\nJoystick is up.\n");
                    break;
                case left:
                    currentDir = left;
                    printf("\r\nJoystick is left.\n");
                    break;
                case right:
                    currentDir = right;
                    printf("\r\nJoystick is right.\n");
                    break;
                case neutral:
                    // Joystick is back to center, but we retain the previous state.
                    break;
                default:
                    break;
            }
        }
		
		if (arrow_pos == 0){
			oled_pos(arrow_pos,0);
			oled_arrow();
			oled_print(menu_game1->text);
			oled_clear_line(arrow_pos+2);
			oled_pos(arrow_pos+2,5);
			oled_print(menu_game2->text);
		}else if (arrow_pos == 2){
			oled_clear_line(arrow_pos-2);
			oled_pos(arrow_pos-2,5);
			oled_print(menu_game1->text);
			oled_pos(arrow_pos,0);
			oled_arrow();
			oled_print(menu_game2->text);
		}
		if (currentDir == down && arrow_pos == 0){
		arrow_pos=2;
		}
		if (currentDir == up && arrow_pos == 2){
		arrow_pos=0;
		}
		
		if (bit_is_set(PINB, PB0)==1)
		{
			acc = 0;
			oled_reset();
		}
		printf("\r\nIf loop for play done");
	}
	printf("\r\nWhile loop 2 done");

	
	}*/
	
	
	
	
	
	/*while (1)
	{
		
		joysticks(joyarr);
		direction(cal);
		
		
		printf("\n\r X_AXIS:");
		joysticks(joyarr);
		printf("%d", joyarr[0]);
		printf("%% - - Y_AXIS:");
		joysticks(joyarr);
		printf("%d", joyarr[1]);
		printf("%% \r\n");
		
		int direx = direction(cal);
		for (int line = 0; line < 8; line++) {
		oled_goto_line(line);
		oled_goto_column(8);
		for (int i = 0; i < 128; i++) {
			oled_write_data(0xFF);
		}
	}
		
		if (direx == 1){
			oled_pos(4,60);
			oled_write_data (~0b00011000 );
			oled_write_data (~0b00011000 );
			oled_write_data (~0b01111110 );
			oled_write_data (~0b00111100 );
			oled_write_data (~0b00011000 );
		}
		if (direx == 2)
		{
		
		oled_pos(4,60);
		oled_write_data (~0b00011000 );
		oled_write_data (~0b00111100 );
		oled_write_data (~0b01111110 );
		oled_write_data (~0b00011000 );
		oled_write_data (~0b00011000 );
	}
	
	PROGMEM font8[16];
	
	

	
	}	
	/*
	for (int line = 0; line < 8; line++) {
		oled_goto_line(line);
		oled_goto_column(8);
		for (int i = 0; i < 128; i++) {
			oled_write_data(0xFF);
			_delay_ms(2);
		}
	}
	
	oled_clear_line(3);*/
	/*while(1){
		
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
		
		printf("\n\r RIGHT SLIDER:");
		sliders(slidarr);
		printf("%d", slidarr[0]);
		printf(" - - LEFT SLIDER:");
		sliders(slidarr);
		printf("%d", slidarr[1]);
		printf("\r\n");
		
		_delay_ms(1000);
	}*/
	
}
