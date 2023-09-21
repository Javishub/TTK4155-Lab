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
#define deadzone 0x08


uint8_t map(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void joysticks(uint8_t joyarr[]){
	uint8_t x_axis = 0x81;
	uint8_t y_axis = 0x80;
	volatile char *adc = (char *) 0x1400;
	
	adc[0] = x_axis;
	_delay_ms(2);
	uint8_t x = adc[0];
	adc[0] = y_axis;
	_delay_ms(2);
	uint8_t y = adc[0];
	
	joyarr[0]=x;
	joyarr[1]=y;
}

void sliders(uint8_t slidarr[]){
	uint8_t right_slider = 0x83;
	uint8_t left_slider = 0x82;
	volatile char *adc = (char *) 0x1400;
	
	adc[0] = right_slider;
	_delay_ms(2);
	uint8_t x = adc[0];
	adc[0] = left_slider;
	_delay_ms(2);
	uint8_t y = adc[0];
	
	slidarr[0]=x;
	slidarr[1]=y;
}

void calibrate(uint8_t arr_val[]){
	uint8_t x_axis = 0x81;
	uint8_t y_axis = 0x80;
	
	volatile char *adc = (char *) 0x1400;
	adc[0] = x_axis;
	uint8_t val_x = adc[0];
	uint8_t deadzone_x_min = val_x -deadzone;
	uint8_t deadzone_x_max = val_x +deadzone;
	arr_val[0] = deadzone_x_min;
	arr_val[1] = deadzone_x_max;
	
	adc[0] = y_axis;
	uint8_t val_y = adc[0];
	uint8_t deadzone_y_min = val_y -deadzone;
	uint8_t deadzone_y_max = val_y +deadzone;
	arr_val[2] = deadzone_y_min;
	arr_val[3] = deadzone_y_max;
}


void direction(uint8_t vals[4]){
	uint8_t joyarr[2];
	joysticks(joyarr);
		if (joyarr[0] > vals[0] && joyarr[0] < vals[1] && joyarr[1] > vals[2] && joyarr[1] < vals[3])
	{	
		printf("NETURAL\r\n");
	}
	 if (joyarr[0] >= vals[1])
	 {
		printf("RIGHT\r\n");
	 }
	 if (joyarr[0] <= vals[0])
	 {
		 printf("LEFT\r\n");
	 }
	  if (joyarr[1] >= vals[3])
	 {
		printf("UP\r\n");
	 }
	 if (joyarr[1] <= vals[2])
	 {
		 printf("DOWN\r\n");
	 }
	 
	 
	return;
}
