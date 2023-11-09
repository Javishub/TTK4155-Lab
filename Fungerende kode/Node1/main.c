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
#include "SRAM_ADC.h"
#include "joysticking.h"
#include "OLED.h"
#include "fonts.h"
#include "menus.h"
#include "SPI.h"
#include "mcp2515.h"
#include "CAN.h"

#define deadzone 0x08


typedef enum {highscore, play_game} currentState_t;
	



int main(void)
{
	UART_Init(MYUBRR);
	//SRAM_init();
	//SRAM_test();
	pwm_init();
	adc_init();
	oled_init();
	mcp_init();
	mcp_set_bitrate();
	printf("\r\nMODE: %x\r\n", mcp_read(MCP_CANSTAT));
	mcp_set_mode(MODE_NORMAL);
	printf("\r\nMODE: %x\r\n", mcp_read(MCP_CANSTAT));
	
	
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
	
	message_t message = {
		1,
		8,
		{0,0,0,0,0,0,0,0}
	};
	message_t score = {
		1,
		7,
		{0,0,0,0,0,0,0,0}
	};
	
	directions_t currentDir = neutral;
	directions_t previousDir = neutral;
	
	menu_ptr menu = malloc(sizeof(menu_t));
	
	menu_ptr menu_highscore = menu_add(menu, "Vis Highscore", &show_highscore);
	menu_ptr menu_play = menu_add(menu, "New Game", &playgame);
	
	menu_ptr menu_game1 = menu_add(menu_play, "Save 1", &play_game1);
	menu_ptr menu_game2 = menu_add(menu_play, "Save 2", &play_game2);
	
	
	int arrow_pos=0;
	int acc=0;
	printf("\r\n--------------------\n");
	
	int scored = 0;
	int prev_scored = 0;
	_delay_ms(1000);
	
	
	while(1){
		directions_t dir = direction(cal);
		_delay_ms(10);
		message.data[3] = 1;
		message.data[0] = 0;
		message.data[1] = 0;
		can_send(&message);
		
		/*score = can_receive();
		if (score.data[0] > 0){
			scored = score.data[0];
			if(prev_scored != scored){
				prev_scored = scored;
				//printf("SCORE: %d\n\r", scored);
				oled_pos(0,0);
				oled_print("Score :");
				oled_print_int(scored);			
			} 
			if (scored == 255){
				oled_pos(0,0);
				oled_clear_line(0);
				oled_print("YOU WON!");
				scored = 0;
				_delay_ms(1000);
				oled_reset();
			}
		}
		
		
		
		
		message.data[0] = joyarr[0];	//X-AKSE
		message.data[1] = joyarr[1];	//Y-AKSE
		if(PINB & (1 << PB1)){
			message.data[3] = 1;
			oled_reset();
		}
		else{
			message.data[3] = 0;
		}
		if(PINB & (1 << PB0)){
			message.data[2] = 1;
		}
		else{
			message.data[2] = 0;
		}
		can_send(&message);*/
		
		//if (dir != previousDir) {
			// Joystick state has changed
			//previousDir = currentDir;

			// Handle state transitions
			/*switch (dir) {
				case neutral:
				message.data[0] = 0;
				break;
				case down:
				message.data[0] = 1;
				break;
				case up:
				message.data[0] = 2 ;
				break;
				case left:
				message.data[0] = 3 ;
				break;
				case right:
				message.data[0] = 4 ;
				break;
				default:
				message.data[0] = 0;
				break;
			}*/
		
		//printf("XAKSE: %d\n\r", message.data[0]);
		
		
		/*uint8_t cnf1 = mcp_read(0x2A);
		uint8_t cnf2 = mcp_read(0x29);
		uint8_t cnf3 = mcp_read(0x28);
		printf("CNF1 %u\n\r", cnf1);
		printf("CNF2 %u\n\r", cnf2);
		printf("CNF3 %u\n\r", cnf3);*/

	
	
	
	
	

	

		
	
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
	
	if (PINB & (1 << PB0) && arrow_pos == 0){
		acc = 1;
 		printf("\r\nHighscore selected");
		oled_reset();
	
	}
	if(PINB & (1 << PB0) && arrow_pos == 2){
 		printf("\r\nPlay selected");
		acc = 2;
		message.data[3] = 1;
		can_send(&message);
		oled_reset();
		_delay_ms(50);
		score = can_receive();
		_delay_ms(50);
		
	}
	
	while (acc == 1)
	{
		oled_pos(0,0);
		menu_highscore->function();
		
		if (PINB & (1 << PB1))
		{
			acc = 0;
			oled_reset();
		}
	}
	while (acc == 2){
		joysticks(joyarr);
		message.data[0] = joyarr[0];	//X-AKSE
		message.data[1] = joyarr[1];	//Y-AKSE
		can_send(&message);
		score = can_receive();
		if (score.data[0] > 0){
			if(score.data[1]>= 200){
				oled_reset();
				oled_print("You suck");
				oled_pos(1,0);
				oled_print("Therefore you lose!");
				update_highscore(scored);
				_delay_ms(2000);
				message.data[0] = 0;
				message.data[1] = 0;
				can_send(&message);
				oled_reset();
				acc = 0;
			}
			scored = score.data[0];
			if(prev_scored != scored){
				prev_scored = scored;
				//printf("SCORE: %d\n\r", scored);
				oled_reset();
				oled_pos(0,0);
				oled_print("Score :");
				oled_print_int(scored);
			}
			if (scored == 255){
				oled_pos(0,0);
				oled_clear_line(0);
				oled_print("YOU WON!");
				scored = 0;
				_delay_ms(1000);
				oled_reset();
			}
	}
	
		
		
		
		
		
		if(PINB & (1 << PB1)){
			message.data[3] = 1;
			oled_reset();
			acc = 0;
		}
		else{
			message.data[3] = 0;
		}
		if(PINB & (1 << PB0)){
			message.data[2] = 1;
		}
		else{
			message.data[2] = 0;
		}
		
		
		//printf("\r\nIf loop for play done");
	}
	//printf("\r\nWhile loop 2 done");

	
	}
	
	
	
	
	
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
	
	for (int line = 0; line < 8; line++) {
		oled_goto_line(line);
		oled_goto_column(8);
		for (int i = 0; i < 128; i++) {
			oled_write_data(0xFF);
			_delay_ms(2);
		}
	}
	
	oled_clear_line(3);
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
		
		
		printf("\n\r X_AXIS:");
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