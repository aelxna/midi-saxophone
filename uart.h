#ifndef UART_H
#define	UART_H

#include <avr/io.h>

void uart_init();

void uart_write_byte(uint8_t byte);

void uart_write_cmd(uint8_t *cmd);

#endif	/* UART_H */

