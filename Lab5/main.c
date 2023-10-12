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
	mcp_set_mode(MODE_LOOPBACK);
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
		"0110"
	};
	
	
	_delay_ms(1000);
	while(1){
	_delay_ms(10);
	
	can_send(&message);
	message_t receive = can_receive();
	printf("ID: %d\r\n", receive.id);
	printf("Lengde: %d\r\n", receive.length);
	printf("Melding: %s\r\n", receive.data);
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
