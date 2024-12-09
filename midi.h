#ifndef MIDI_H
#define MIDI_H

#include <avr/io.h>

#include "gpio_i2c.h"

#define MAP_LEN 32
#define NOTE_MAPPING(idx) (idx + 49)

// constants used for velocity
#define NOTE_OFF 0
#define PIANO 31
#define MEZZO_PIANO 63
#define MEZZO_FORTE 95
#define FORTE 127

// constants used for midi messages
#define NOTE_ON 0x90
#define PROG_CHANGE 0xC0
#define START 0xFA
#define CLOCK 0xF8
#define SYSEX_ON 0xF0
#define SYSEX_OFF 0xF7
#define ALTO_SAX 65

typedef uint32_t mapping_t;

// call setup midi commands (program change to alto sax)
void setup_midi_device();

// return the note number based on the input keys
uint8_t get_note(inputs_t *inputs);

// return the dynamic based on the adc reading
uint8_t get_velocity(uint16_t adc);

// send a note on message with the given note and velocity
// turn off the previous note when switching notes
void send_note(uint8_t note, uint8_t velocity, uint8_t new_note, mapping_t prev);

// used to send a sysex message
// not used in our program, but available for future sysex use
void send_sysex(uint8_t *id, int id_bytes, uint8_t *data, int data_bytes);

#endif /* MIDI_H */
