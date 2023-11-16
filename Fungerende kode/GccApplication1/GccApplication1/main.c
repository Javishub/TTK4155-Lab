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
	
	menu_ptr Gameloss = menu_add(menu_play, "Gameloss", &game_loss);
	menu_ptr Gamewon = menu_add(menu_play, "Won game", &won_game);
	menu_ptr Playing = menu_add(menu_play, "Game running", &playing_game);
	
	
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
		oled_print(menu_highscore->text);	//Moving arrow to the correct menu select line
		oled_clear_line(arrow_pos+2);
		oled_pos(arrow_pos+2,5);
		oled_print(menu_play->text);
	}else if (arrow_pos == 2)
	{
		oled_clear_line(arrow_pos-2);
		oled_pos(arrow_pos-2,5);
		oled_print(menu_highscore->text);	//Moving arrow to the correct menu select line
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
	
	if (PINB & (1 << PB0) && arrow_pos == 0){	//when button pressed and arrow at menu line 0 select option 0
		acc = 1;
		oled_reset();
	
	}
	if(PINB & (1 << PB0) && arrow_pos == 2){		//when button pressed and arrow at menu line 2 select option 2
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
		
		if (PINB & (1 << PB1))	//exit menu
		{
			acc = 0;
			oled_reset();
		}
	}
	while (acc == 2){					//playing the game
		joysticks(joyarr);
		message.data[0] = joyarr[0];	//X-AKSE
		message.data[1] = joyarr[1];	//Y-AKSE
		can_send(&message);
		score = can_receive();
		if (score.data[0] > 0){
			if(score.data[1]>= 200){
				Gameloss->function();
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
				Playing->function(scored);
			}
			if (scored == 255){
				Gamewon->function();
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
	}
	}
}