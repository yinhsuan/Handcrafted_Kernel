#include "shell.h"
#include "uart1.h"

void cli_print_banner() {
    uart_puts("=======================================\r\n");
    uart_puts("  Welcome to NYCU-OSC 2023 Lab1 Shell  \r\n");
    uart_puts("=======================================\r\n");
}

void cli_cmd_clear(char* buffer, int length) {
    for (int i=0; i<length; i++) {
        buffer[i] = '\0';
    }
}