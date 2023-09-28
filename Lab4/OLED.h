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


void oled_init() {
	oled_write_command(0xae); // display off
	oled_write_command(0xa1); //segment remap
	oled_write_command(0xda); //common pads hardware: alternative
	oled_write_command(0x12);
	oled_write_command(0xc8); //common output scan direction:com63~com0
	oled_write_command(0xa8); //multiplex ration mode:63
	oled_write_command(0x3f);
	oled_write_command(0xd5); //display divide ratio/osc. freq. mode
	oled_write_command(0x80);
	oled_write_command(0x81); //contrast control
	oled_write_command(0x50);
	oled_write_command(0xd9); //set pre-charge period
	oled_write_command(0x21);
	oled_write_command(0x20); //Set Memory Addressing Mode
	oled_write_command(0x02);
	oled_write_command(0xdb); //VCOM deselect level mode
	oled_write_command(0x30);
	oled_write_command(0xad); //master configuration
	oled_write_command(0x00);
	oled_write_command(0xa4); //out follows RAM content
	oled_write_command(0xa6); //set normal display
	oled_write_command(0xaf); // display on
}

void oled_write_command(uint8_t command) {
	volatile char *ext_oled_command = (char *) 0x1000;
	ext_oled_command[0] = command;
}

void oled_write_data(uint8_t data) {
	volatile char *ext_oled_data = (char *) 0x1200;
	ext_oled_data[0] = data;
}

void oled_reset(){
	oled_write_command(0x21);
	oled_write_command(0);
	oled_write_command(127);
	for (int line = 0; line < 8; line++) {
		oled_goto_line(line);
		oled_goto_column(0);
		for (int i = 0; i < 128; i++) {
			oled_write_data(0x0);
		}
	}
	oled_pos(0,0);
}

void oled_clear_line(int line){
		oled_goto_line(line);
		oled_goto_column(0);
		for (int i = 0; i < 128; i++) {
			oled_write_data(0x0);
		}
}

void oled_home(){
	oled_pos(0,0);
}

void oled_brightness (int br){
	br = ((br - 0) * (255 - 0) / (100 - 0) + 255);
	oled_write_command(0x81);
	oled_write_command(br);
}


void oled_pos(int line, int column){
	oled_goto_line(line);
	oled_goto_column(column);
}

void oled_goto_column(int column) {
	oled_write_command(0x21);
	oled_write_command(column);
	oled_write_command(127);
	
}
void oled_goto_line(int line) {
  oled_write_command(0xB0 + line);
}



