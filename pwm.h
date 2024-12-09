#ifndef PWM_H
#define	PWM_H

#include <avr/io.h>
#include "gpio_i2c.h"

#ifndef F_CPU
#define F_CPU 3333333
#endif

// returns the frequency based on the current key mapping
uint16_t get_freq(inputs_t * inputs);

// initial setup for speaker
void setupSpeaker();

// convert frequency(Hz) to period
int hertzToPeriod(int hertz);

// set period of the tca peripheral
void setPeriod(int period);

void enableSpeaker();

void disableSpeaker();

#endif	/* PWM_H */

