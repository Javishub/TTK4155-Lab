// pulse width 0.9ms - 2.1ms
// signal period = 20ms
// duty cycle = 0.9/20 <-> 2.1 / 20
// duty cycle = 0.045 <-> 0.105

#define PERIODE 0.02
#define DIVA 84
//#define CPRD (int) (PERIODE*CHIP_FREQ_CPU_MAX/DIVA)
const int CPRD = (PERIODE*CHIP_FREQ_CPU_MAX/DIVA);

//#define min_duty_cycle (double) 0.9/20
//#define max_duty_cycle (double) 2.1/20

const double min_duty_cycle = 0.9/20;
const double max_duty_cycle = 2.1/20;

#define X_MIN 0
#define X_MAX 255

#define GAIN (max_duty_cycle - min_duty_cycle)/(X_MIN-X_MAX)
#define OFFSET max_duty_cycle - X_MIN * GAIN

float map(int x, int in_min, int in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int pwm_init(){
	
	//PWM->PWM_WPCR = ('p'<<24) | ('w'<<16) | ('m'<<8) | ((0b111111<<2)+0b00);
	
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;
	while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
	{
	};
	
	//PMC->PMC_PCR = PMC_PCR_CMD | PMC_PCR_EN | 36;
	
	PIOC->PIO_ABSR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;	//Setter P19 og P18 til B funksjon
	PIOC->PIO_PDR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;		//Disabler PIO fra å kontrollere pinne 18 og 19
	
	
	PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(DIVA);
	PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_CLKA;		//P18
	PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_CLKA;		//P19
	
	PWM->PWM_CH_NUM[5].PWM_CPRD = PWM_CPRD_CPRD(CPRD);
	//PWM->PWM_CH_NUM[5].PWM_CPRD = 100;
	PWM->PWM_ENA = PWM_ENA_CHID5;
	
	PWM->PWM_CH_NUM[6].PWM_CPRD = PWM_CPRD_CPRD(CPRD);
	PWM->PWM_ENA = PWM_ENA_CHID6;
	
	
	//PWM->PWM_WPCR = ('p'<<24) | ('w'<<16) | ('m'<<8) | ((0b111111<<2)+0b10);
	
}

void pwm_set_dutycycle(double duty_cycle){
	duty_cycle = duty_cycle/20;
	
	 if (duty_cycle > max_duty_cycle)
	 {
		 duty_cycle = max_duty_cycle;
	 }
	 if (duty_cycle < min_duty_cycle)
	 {
		 duty_cycle = min_duty_cycle;
	 }
	 int CDTY = 0;
	 //CDTY = (int) (PWM->PWM_CH_NUM[5].PWM_CPRD*(1-duty_cycle));
	 //CDTY = -1/(PERIODE*(CHIP_FREQ_CPU_MAX/DIVA)*((duty_cycle)-1));
	 CDTY = CPRD-duty_cycle*CHIP_FREQ_CPU_MAX/(50*DIVA);		//DIY likning men det eneste som funker
	 //int p=20;
	 //CDTY = (p - 1)/((CHIP_FREQ_CPU_MAX/DIVA)*duty_cycle*p);
	 //printf("CDTY: %d\n\r", CDTY);
	 //printf("CPRD: %d\n\r", CPRD);
	 //PWM->PWM_CH_NUM[5].PWM_CDTY = PWM_CDTY_CDTY(CDTY);
	 PWM->PWM_CH_NUM[5].PWM_CDTY = CDTY;
	 PWM->PWM_CH_NUM[6].PWM_CDTY = CDTY;
}

void servo_pos(int x){
	float dutycycle = map(x,X_MIN,X_MAX,2.1,0.9);
	if(x < 169 && x > 157){
		dutycycle = 1.5;
	}
	pwm_set_dutycycle(dutycycle);
}

void servo_init(){
	servo_pos(0);
}
