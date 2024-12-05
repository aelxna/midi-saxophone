/*
 * File:   5143_FinalProject_Main.c
 * Author: gahym
 *
 * Created on November 12, 2024, 10:32 AM
 */

#include "adc.h"
#include "gpio_i2c.h"
#include "midi.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/cpufunc.h>

inputs_t currInputs;
mapping_t prevMapping;

int setup(void) {
  // disable cpu prescaling so the clock runs at 20 MHz
//  ccp_write_io(&CLKCTRL.MCLKCTRLB, 0x00);
    
  uart_init();
  setupDisplay();
  setupADC();
  setupSpeaker();
  PORTD.DIRCLR = PIN1_bm;
  PORTD.PIN1CTRL = PORT_PULLUPEN_bm;

  sei();
}

int hertzToPeriod (int hertz) {
    return 3333333 / hertz;
}

void setupSpeaker() {
    PORTC.DIRSET = PIN0_bm;
    
    TCA0.SINGLE.CTRLESET = TCA_SINGLE_CMD_RESET_gc;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_FRQ_gc;
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTC_gc;
    TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);
    
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;
    TCA0.SINGLE.PER = 7574; // 52081
    TCA0.SINGLE.CMP0 = 3786;
    
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

ISR(ADC0_RESRDY_vect) {
  /* Clear flag by writing '1': */
  ADC0.INTFLAGS = ADC_RESRDY_bm;

  currInputs.airflow = ADC0.RES;

  //setDisplay(currInputs.airflow / 100, currInputs.airflow % 100, 0);
}

int main(void) {
  /* Replace with your application code */
  setup();
  
  int hertz = 440;

  configureExpander();

  setDisplay(0, 0, 0);
  
  _delay_ms(200);
  setup_midi_device();

  // displayCurrentNote('a', 3, 1);

  uint32_t pinData;
  uint8_t new_note = 0;

  while (1) {

    pinData = readPins();
    
    
    if (pinData & 0x00F0) {
      pinData |= 0x10000;
    } else {
      pinData &= ~0x10000;
    }

    if (!(PORTD.IN & PIN1_bm)) {
      pinData |= 0x0080; // pin 7
    } else {
      pinData &= ~0x0080;
    }
    
    currInputs.keys = pinData;
    
//    displayHex(pinData);

    // run input checker
    uint8_t note = get_note(&currInputs);
    uint16_t noteFrequency = get_note(&currInputs);
    uint8_t vel = get_velocity(currInputs.airflow);
    if (currInputs.keys == prevMapping) {
      new_note = 1;
    }
    // do stuff with input
    //send_note(note, vel, new_note, prevMapping);
    displayNoteAndVelocity(note, vel);
    prevMapping = note;
    new_note = 0;
    
    hertz = 1000;
    int period = hertzToPeriod(noteFrequency);
    TCA0.SINGLE.PERBUF = period;
    TCA0.SINGLE.CMP0BUF = period/2;
    
  }
}
