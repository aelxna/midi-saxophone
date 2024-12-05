#include "uart.h"

uart_pin_t uart_pin = {.dir = &(PORTC.DIR), .pin = PIN0_bm};

void uart_init() {
    //*(uart_pin.dir) |= uart_pin.pin; // TX
    PORTC.DIRSET = PIN0_bm;
    
    USART1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;
    USART1.BAUD = (uint16_t)USART_BAUD_VALUE(31250); // set baud rate
    USART1.CTRLB |= USART_TXEN_bm; // enable transmit
}

void uart_write_byte(uint8_t byte) {
    while(!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = byte;
}

void uart_write_cmd(uint8_t *cmd, int len) {
    for (uint8_t i = 0; i < len; i++) {
        uart_write_byte(cmd[i]);
    }
}
