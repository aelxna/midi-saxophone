#include "adc.h"


void setupADC (void) {
    
    PORTD.DIRCLR = ADCPin;
    PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
     
    /* Disable digital input buffer */
    PORTD.PIN6CTRL &= ~PORT_ISC_gm;
    PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;

    /* Select ADC channel */
    ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc; // set pin to be D6
    ADC0.CTRLC |= ADC_PRESC_DIV4_gc; /* CLK_PER divided by 4 */    
    ADC0.CTRLC |= ADC_REFSEL_VDDREF_gc; /* VDD reference */    
    ADC0.CTRLA |= ADC_RESSEL_10BIT_gc; /* 10-bit mode */    
    ADC0.INTCTRL |= ADC_RESRDY_bm; 
    ADC0.CTRLA |= ADC_ENABLE_bm; /* ADC Enable: enabled */
    ADC0.CTRLA |= ADC_FREERUN_bm;
    
    // turn off the ADC conversion 
    ADC0.COMMAND &= ~ADC_STCONV_bm;
    
    
}
