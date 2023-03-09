#include "bcm2837/rpi_gpio.h"
#include "bcm2837/rpi_uart1.h"
#include "uart1.h"

void uart_init() {
    // register unsigned int r;

    /* initialize UART */
    *AUX_ENABLES     |= 1;       // enable UART1 -> // Set AUXENB register to enable mini UART. Then mini UART register can be accessed.
    *AUX_MU_CNTL_REG  = 0;       // disable TX/RX -> // Disable transmitter and receiver during configuration.

    /* configure UART */
    *AUX_MU_IER_REG   = 0;       // disable interrupt -> // Disable interrupt because currently you don’t need interrupt.
    *AUX_MU_LCR_REG   = 3;       // 8 bit data size
    *AUX_MU_MCR_REG   = 0;       // disable flow control
    *AUX_MU_BAUD_REG  = 270;     // 115200 baud rate -> // Set baud rate to 115200
    *AUX_MU_IIR_REG   = 6;       // disable FIFO



    /* finish configure UART */
    *AUX_MU_CNTL_REG = 3;      // enable TX/RX
}

char uart_recv() {
    char r;
    while (!(*AUX_MU_LSR_REG & 0x01)) {}; // Check AUX_MU_LSR_REG’s data ready field
    r = (char) (*AUX_MU_IO_REG); // If set, read from AUX_MU_IO_REG
    return r == '\r' ? '\n' : r;
}

void uart_send (unsigned int c) {
    while (!(*AUX_MU_LSR_REG & 0x20)) {};
    *AUX_MU_IO_REG = c;
}

void uart_puts(char *str) {
    while (*str) {
        if (*str == '\n') {
            uart_send('\r');
        }
        uart_send(*str++);
    }
}