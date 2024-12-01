#include "uart.h"

void uart_init() {
    *(uart_pin.dir) |= *(uart_pin.pin); // TX
    USART0.BAUD = (uint16_t)USART_BAUD_VALUE(31250); // set baud rate
    USART0.CTRLB |= USART_TXEN_bm; // enable transmit
}

void uart_write_byte(uint8_t byte) {
    while(!(USART0.STATUS & USART_DREIF_bm));
    USART0.TXDATAL = byte;
}

void uart_write_cmd(uint8_t *cmd, int len) {
    for (uint8_t i = 0; i < len; i++) {
        uart_write_byte(cmd[i]);
    }
}
