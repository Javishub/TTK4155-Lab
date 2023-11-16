/*
 * motorcontrol.h
 *
 * Created: 04.11.2023 12:36:09
 *  Author: larskrud
 */ 


#define NOT_OE	PIO_PD0
#define NOT_RST PIO_PD1
#define SEL		PIO_PD2
#define EN		PIO_PD9
#define DIR		PIO_PD10

#define DO0_IDX		1
#define MOTOR_FREQ	50

#define ENCODER_MASK	(0xFF << DO0_IDX)
#define MIN_ENCODER_VAL	0
#define MAX_ENCODER_VAL	308


const int MAX_SPEED = 4;


enum motor_dir{
	LEFT,
	RIGHT
	};
	
void dac_write(uint16_t data){
	DACC->DACC_CDR = data;
	
	while (!DACC->DACC_ISR & DACC_ISR_EOC);
}

	
void dac_init(){
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_DACC << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << (ID_DACC - 32);
	
	DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_WORD_HALF | DACC_MR_STARTUP_0; //Select ch1, half word transfer, 0 periods startuptime
	
	DACC->DACC_CHER = DACC_CHER_CH1;
	
	dac_write(0);
}

	
static void set_motor_speed(int speed){
	dac_write(speed);
}

static void set_motor_dir(enum motor_dir direction){
	if (direction == LEFT){
		PIOD->PIO_CODR = DIR;
		}
	else if (direction == RIGHT){
		PIOD->PIO_SODR = DIR;
	}
}

void motor_init(){
	dac_init();
	
	PIOD->PIO_PER |= DIR | EN | SEL | NOT_RST | NOT_OE;	//Set pins enable
	PIOD->PIO_OER |= DIR | EN | SEL | NOT_RST | NOT_OE;	//Set as outputs
	
	PIOC->PIO_PER |= ENCODER_MASK; //Set pins enabled for encoder input
	PIOC->PIO_ODR |= ENCODER_MASK; //Disable output for encoder pins
	
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PIOC << PMC_PCR_PID_Pos);
	PMC->PMC_PCER0 |= 1 << (ID_PIOC);
}

void disable_motor(){
	set_motor_speed(0);
	PIOD->PIO_CODR = EN;
}

void enable_motor(){
	PIOD->PIO_SODR = EN;
	
}

int read_motor_encoder(){
	PIOD->PIO_CODR |= NOT_OE;
	PIOD->PIO_CODR |= SEL;
	delay_us(20);
	uint8_t msb = (PIOC->PIO_PDSR & ENCODER_MASK) >> DO0_IDX;
	
	PIOD->PIO_SODR |= SEL;
	delay_us(20);
	uint8_t lsb = (PIOC->PIO_PDSR & ENCODER_MASK) >> DO0_IDX;
	
	PIOD->PIO_CODR |=NOT_RST;
	PIOD->PIO_SODR |=NOT_RST;
	
	PIOD->PIO_SODR |= NOT_OE;
	
	uint16_t encoder_data = ((msb << 8) | lsb);
	if (encoder_data & (1 << 15)){
		return ((uint16_t) (~encoder_data) + 1);
	}
	return ~encoder_data;
}

//	Speedbased motorcontrol
void joystick_speedcontrol(int joystick){
	int speed = 0;
	if (joystick > 170){
		set_motor_dir(RIGHT);
		speed = map(joystick,175,255,1,MAX_SPEED);
	}
	else{
		set_motor_dir(LEFT);
		speed = map(joystick,0,165,MAX_SPEED,1);
	}
	set_motor_speed(0x4FF*speed);
}

void joystick_motoring(int joystick){
	
	int pos = ((map(joystick, 0, 162,MIN_ENCODER_VAL,MAX_ENCODER_VAL/2) + map(joystick, 163, 230,(MAX_ENCODER_VAL/2)+1,MAX_ENCODER_VAL))/2);	//Mapping value coordinates from joystick to encoder values
	if(pos < 0){
		pos = 0;
	}
	int pidscale = abs(pid(pos,read_motor_encoder()));
	
	if(pos >= read_motor_encoder()){
		set_motor_dir(RIGHT);
		set_motor_speed(pidscale);
	}
	if(pos <= read_motor_encoder()){
		set_motor_dir(LEFT);
		set_motor_speed(pidscale);
	}	
}
