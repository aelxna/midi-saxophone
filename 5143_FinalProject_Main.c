/*
 * File:   5143_FinalProject_Main.c
 * Author: gahym
 *
 * Created on November 12, 2024, 10:32 AM
 */


#include <avr/io.h>
#include "adc.h"
#include "gpio_i2c.h"
#include <avr/interrupt.h>

inputs_t currInputs;

int setup (void) {
    
   
    sei();
    
}

ISR(ADC0_RESRDY_vect)
{
    /* Clear flag by writing '1': */
    ADC0.INTFLAGS = ADC_RESRDY_bm;
   
    currInputs.airflow = ADC0.RES;
    

 
}

int main(void) {
    /* Replace with your application code */
    while (1) {
        
        // run input checker
        // do stuff with input
        
    }
}
