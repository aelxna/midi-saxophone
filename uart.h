#ifndef UART_H
#define UART_H

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 3333333
#endif

#define SAMPLES_PER_BIT 16
#define USART_BAUD_VALUE(BAUD_RATE)                                            \
  (uint16_t)((F_CPU << 6) / (((float)SAMPLES_PER_BIT) * (BAUD_RATE)) + 0.5)

// uart pin shenanigans
typedef struct {
  register8_t *dir;
  uint8_t pin;
} uart_pin_t;

// initialize the uart pins and settings
void uart_init();

// send a single byte over uart
void uart_write_byte(uint8_t byte);

// send an array of bytes over uart
void uart_write_cmd(uint8_t *cmd, int len);

#endif /* UART_H */
