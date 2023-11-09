/*
 * ADC.h
 *
 * Created: 02.11.2023 15:56:08
 *  Author: larskrud
 */ 

void adc_init(){
	ADC->ADC_MR = ADC_MR_FREERUN;
	ADC->ADC_CHER = ADC_CHER_CH0;
	
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_ADC << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1<<(ID_ADC - 32);
	
	ADC->ADC_CR = ADC_CR_START;
}

uint16_t adc_read(){
	return ADC->ADC_CDR[0];
}