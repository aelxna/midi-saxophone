#include "midi.h"
#include "uart.h"

void setup_midi_device() {
    // change channel 0 to alto sax
    uint8_t prog_change[] = {
        0xC0,
        66
    };
    uart_write_cmd(prog_change, 2);
}

uint8_t get_note(inputs_t *inputs) {
    for (int i = 0; i < MAP_LEN; i++) {
        if (inputs->keys == map[i]) {
            return NOTE_MAPPING(i);
        }
    }
}

uint8_t get_velocity(uint16_t adc) {
    if (adc > 818) {
        return FORTE;
    } else if (adc > 613) {
        return MEZZO_FORTE;
    } else if (adc > 408) {
        return MEZZO_PIANO;
    } else if (adc > 203) {
        return PIANO;
    } else {
        return NOTE_OFF;
    }
}

void send_note(uint8_t note, uint8_t velocity) {
    uint8_t cmd[] {
        0x90,
        note,
        velocity
    };
    uart_write_cmd(cmd, 3);
}

void send_sysex(uint8_t *id, int id_bytes, uint8_t *data, int data_bytes) {
    // send sysex command
    uart_write_byte(0xF0);
    // send identifier bytes
    uart_write_cmd(id, id_bytes);
    // send data
    uart_write_cmd(data, data_bytes);
    // send sysex stop byte
    uart_write_byte(0xF7);
}
