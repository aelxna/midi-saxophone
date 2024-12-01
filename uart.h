#ifndef UART_H
#define	UART_H

#include <avr/io.h>

#define F_CPU 3333333
#define SAMPLES_PER_BIT 16
#define USART_BAUD_VALUE(BAUD_RATE) (uint16_t) ((F_CPU << 6) / (((float) SAMPLES_PER_BIT) * (BAUD_RATE)) + 0.5)

typedef struct {
    register8_t *dir;
    uint8_t pin;
} uart_pin_t;

uart_pin_t uart_pin = {
    .dir = &(PORTA.DIR),
    .pin = PIN0_bm
};

void uart_init();

void uart_write_byte(uint8_t byte);

void uart_write_cmd(uint8_t *cmd, int len);

#endif	/* UART_H */

