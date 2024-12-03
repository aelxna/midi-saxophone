#ifndef MIDI_H
#define MIDI_H

#include "gpio_i2c.h"
#include <avr/io.h>

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
#define SYSEX_ON 0xF0
#define SYSEX_OFF 0xF7
#define ALTO_SAX 66

typedef uint32_t mapping_t;

// fun!
const mapping_t map[] = {
    0x000008bf, // A#/Bb1
    0x000004bf, // B1
    0x000000bf, // C2
    0x000002bf, // C#/Db2
    0x0000003f, // D2
    0x0000007f, // D#/Eb2
    0x0000001f, // E2
    0x0000000f, // F2
    0x00000017, // F#/Gb2
    0x00000007, // G2
    0x00000107, // G#/Ab2
    0x00000003, // A2
    0x00001003, // A#/Bb2
    0x00000001, // B2
    0x00000002, // C3
    0x00000000, // C#/Db3
    0x0001003f, // D3
    0x0001007f, // D#/Eb3
    0x0001001f, // E3
    0x0001000f, // F3
    0x00010017, // F#/Gb3
    0x00010007, // G3
    0x00010107, // G#/Ab3
    0x00010003, // A3
    0x00011003, // A#/Bb3
    0x00010001, // B3
    0x00010002, // C4
    0x00010000, // C#/Db4
    0x00014000, // D4
    0x00016000, // D#/Eb4
    0x00017000, // E4
    0x0001f000  // F4
};

// only need to call the note on message once
uint8_t first_note_on = 1;

// call setup midi commands (program change to alto sax)
void setup_midi_device();

// return the note number based on the input keys
uint8_t get_note(inputs_t *inputs);

// return the dynamic based on the adc reading
uint8_t get_velocity(uint16_t adc);

// send a note on message with the given note and velocity
// turn off the previous note when switching notes
void send_note(uint8_t note, uint8_t velocity, uint8_t new_note,
               mapping_t prev);

// used to send a sysex message
// not used in our program, but available for future sysex use
void send_sysex(uint8_t *id, int id_bytes, uint8_t *data, int data_bytes);

#endif /* MIDI_H */
