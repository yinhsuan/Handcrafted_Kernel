#ifndef _UART1_H_
#define _UART1_H_

void uart_init();
char uart_recv();
void uart_send(unsigned int c);
void uart_puts(char* str);

#endif