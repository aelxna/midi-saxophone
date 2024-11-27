#ifndef MIDI_H
#define	MIDI_H

#include <avr/io.h>
#include "gpio_i2c.h"

#define MAP_LEN 31
#define NOTE_MAPPING(idx) (idx + 49)

typedef uint32_t mapping_t;

const mapping_t map[] = {
    
};

uint8_t get_note(inputs_t *inputs);

uint8_t get_velocity(uint16_t adc);

void send_note(uint8_t note, uint8_t velocity);

void send_sysex(uint8_t byte);

#endif	/* MIDI_H */
