/*
 * shooter.h
 *
 * Created: 04.11.2023 14:51:41
 *  Author: larskrud
 */ 


static int flag = 0;

void solenoid_init(){
	PIOA->PIO_CODR |= PIO_PA16;
	PIOA->PIO_PER  |= PIO_PA16;
	PIOA->PIO_OER  |= PIO_PA16;
	PIOA->PIO_SODR |= !PIO_PA16;
}

void solenoid_shoot(int butt){
	if (butt == 1){
		//flag = 1;
		PIOA->PIO_CODR |= PIO_PA16;
		PIOA->PIO_SODR |= PIO_PA16;
		//_delay_ms(80); //Length of pulse
		PIOA->PIO_CODR |= PIO_PA16;
		PIOA->PIO_SODR |= !PIO_PA16;
	}
	return;
	/*else if(!butt){
		flag = 0;
	}*/
}