/*
 * File:   5143_FinalProject_Main.c
 * Author: gahym
 *
 * Created on November 12, 2024, 10:32 AM
 */


#include <avr/io.h>
#include "adc.h"
#include "gpio_i2c.h"
#include "uart.h"
#include "midi.h"
#include <avr/interrupt.h>
#include <util/delay.h>

inputs_t currInputs;

int setup (void) {
    uart_init();
    setupDisplay();
    //setupADC();
    PORTF.DIRSET = PIN4_bm;
    
    setup_midi_device();
    
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
        uint8_t note = get_note(&currInputs);
        uint8_t vel = get_velocity(currInputs.airflow);
        // do stuff with input
        send_note(note, vel);
    }
}
