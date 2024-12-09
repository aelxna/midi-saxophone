#ifndef PWM_H
#define	PWM_H

#include <avr/io.h>
#include "gpio_i2c.h"

#ifndef F_CPU
#define F_CPU 3333333
#endif

uint16_t get_freq(inputs_t * inputs);

void setupSpeaker();

int hertzToPeriod(int hertz);

void setPeriod(int period);

#endif	/* PWM_H */

