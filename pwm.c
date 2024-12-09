#include "pwm.h"
#include "midi.h"

extern const mapping_t map[];

const uint16_t freqs[] = {138, 146, 155, 164, 174, 185, 196, 207, 220, 233, 246,
                          261, 277, 293, 311, 329, 349, 369, 392, 415, 440, 466,
                          493, 523, 554, 587, 622, 659, 698, 739, 783, 830};

uint16_t get_freq(inputs_t *inputs) {
    for (int i = 0; i < MAP_LEN; i++) {
        if (inputs->keys == map[i]) {
            return freqs[i];
        }
    }
    return freqs[0];
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

int hertzToPeriod(int hertz) {
    return F_CPU / hertz;
}

void setPeriod(int period) {
    TCA0.SINGLE.PERBUF = period;
    TCA0.SINGLE.CMP0BUF = period/2;
}

void enableSpeaker() {
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void disableSpeaker() {
    TCA0.SINGLE.CTRLA &= ~(TCA_SINGLE_ENABLE_bm);
}