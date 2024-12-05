#include "midi.h"
#include "uart.h"

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

void setup_midi_device() {
  // change channel 0 to alto sax
  uint8_t prog_change[] = {PROG_CHANGE, ALTO_SAX};
  uart_write_cmd(prog_change, 2);
}

uint8_t get_note(inputs_t *inputs) {
  for (int i = 0; i < MAP_LEN; i++) {
    if (inputs->keys == map[i]) {
      return NOTE_MAPPING(i);
    }
  }
  return NOTE_MAPPING(0);
}

uint8_t get_velocity(uint16_t adc) {
    return FORTE;
  if (adc > 800) {
    return FORTE;
  } else if (adc > 640) {
    return MEZZO_FORTE;
  } else if (adc > 480) {
    return MEZZO_PIANO;
  } else if (adc > 320) {
    return PIANO;
  } else {
    return NOTE_OFF;
  }
}

void send_note(uint8_t note, uint8_t velocity, uint8_t new_note, mapping_t prev) {
  // shut off prev_note
  // note on w/ velocity 0 is an alias for note off
  if (new_note) {
    uint8_t off_data[] = {NOTE_ON, prev, NOTE_OFF};
    uart_write_cmd(off_data, 3);
  }

  uint8_t note_data[] = {NOTE_ON, note, velocity};
  uart_write_cmd(note_data, 3);
}

void send_sysex(uint8_t *id, int id_bytes, uint8_t *data, int data_bytes) {
  // send sysex command
  uart_write_byte(SYSEX_ON);
  // send identifier bytes
  uart_write_cmd(id, id_bytes);
  // send data
  uart_write_cmd(data, data_bytes);
  // send sysex stop byte
  uart_write_byte(SYSEX_OFF);
}
