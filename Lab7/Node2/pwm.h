// pulse width 0.9ms - 2.1ms
// signal period = 20ms
// duty cycle = 0.9/20 <-> 2.1 / 20
// duty cycle = 0.045 <-> 0.105


int pwm_init(){
	
	REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);
	REG_PWM_CMR0 = PWM_CMR_CALG | PWM_CMR_CPRE_CLKA;
	REG_PWM_CPRD0
	REG_PWM_CDTY0
	
	
	REG_PWM_ENA = PWM_ENA_CHID0;
	
	REG_PWM_CMR0 = 0b1011;
	
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;
	while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
	{
	};
	
	
	
}
