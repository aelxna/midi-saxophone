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
#include <util/delay.h>

inputs_t currInputs;

int setup (void) {
    
    setupDisplay();
    //setupADC();
    PORTF.DIRSET = PIN4_bm;
    
    sei();
    
}

ISR(ADC0_RESRDY_vect)
{
    /* Clear flag by writing '1': */
    ADC0.INTFLAGS = ADC_RESRDY_bm;
   
    currInputs.airflow = ADC0.RES;
    
    setDisplay(currInputs.airflow/100,currInputs.airflow%100,0);
}

int main(void) {
    /* Replace with your application code */
    setup();
  
    configureExpander();
    
    setDisplay(0,0,0);
    
    //displayCurrentNote('a', 3, 1);
    
    uint32_t pinData;
    
    while (1) {

        pinData = readPins();
        currInputs.keys = pinData;

        
        
        displayHex(pinData);
        
        // run input checker
        // do stuff with input
        
    }
}
