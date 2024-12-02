#ifndef MIDI_H
#define	MIDI_H

#include <avr/io.h>
#include "gpio_i2c.h"

#define MAP_LEN 31
#define NOTE_MAPPING(idx) (idx + 49)

#define NOTE_OFF 0
#define PIANO 31
#define MEZZO_PIANO 63
#define MEZZO_FORTE 95
#define FORTE 127

#define NOTE_ON 0x90
#define PROG_CHANGE 0xC0
#define SYSEX_ON 0xF0
#define SYSEX_OFF 0xF7
#define ALTO_SAX 66

typedef uint32_t mapping_t;

const mapping_t map[] = {
    
};

uint8_t first_note_on = 1;

void setup_midi_device();

uint8_t get_note(inputs_t *inputs);

uint8_t get_velocity(uint16_t adc);

void send_note(uint8_t note, uint8_t velocity, uint8_t new_note, mapping_t prev);

void send_sysex(uint8_t *id, int id_bytes, uint8_t *data, int data_bytes);

#endif	/* MIDI_H */

