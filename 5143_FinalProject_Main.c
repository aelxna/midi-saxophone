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

inputs_t currInputs;
mapping_t prevMapping;

int setup(void) {
  uart_init();
  setupDisplay();
  setupADC();
  PORTD.DIRCLR = PIN1_bm;
  PORTD.PIN1CTRL = PORT_PULLUPEN_bm;

  setup_midi_device();

  sei();
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

  configureExpander();

  setDisplay(0, 0, 0);

  // displayCurrentNote('a', 3, 1);

  
  uint32_t pinData;
  uint8_t new_note = 0;

  while (1) {

    pinData = readPins();
    currInputs.keys = pinData;

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

    //displayHex(pinData);

    // run input checker
    uint8_t note = get_note(&currInputs);
    uint8_t vel = get_velocity(currInputs.airflow);
    if (currInputs.keys == prevMapping) {
      new_note = 1;
    }
    // do stuff with input
    send_note(note, vel, new_note, prevMapping);
    displayNoteAndVelocity(note, vel);
    prevMapping = note;
    new_note = 0;
  }
}
