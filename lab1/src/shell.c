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

void cli_cmd_read(char* buffer) {
    char c = '\0';
    int idx = 0;

    while (1) {
        if (idx >= CMD_MAX_LEN) break;

        c = uart_recv();
        if (c == '\n') {
            uart_puts("\r\n");
            break;
        }
        if (16 < c && c < 32) continue;
        if (c > 127) continue;

        buffer[idx++] = c;
        uart_send(c);
    }
}

int cli_cmd_strcmp(const char* p1, const char* p2) {
    const unsigned char *s1 = (const unsigned char*) p1;
    const unsigned char *s2 = (const unsigned char*) p2;
    unsigned char c1, c2;

    do {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c1 == '\0') return c1 - c2;
    } while (c1 == c2);

    return c1 - c2;
}


void cli_cmd_exec(char* buffer) {
    if (cli_cmd_strcmp(buffer, "hello") == 0) {
        do_cmd_hello();
    }
    else if (cli_cmd_strcmp(buffer, "help") == 0) {
        do_cmd_help();
    }
    else if (cli_cmd_strcmp(buffer, "info") == 0) {
        do_cmd_info();
    }
    else if (cli_cmd_strcmp(buffer, "reboot") == 0) {
        do_cmd_reboot();
    }
    else if (*buffer) {
        uart_puts(buffer);
        uart_puts(": command not found\r\n");
    }
}

void do_cmd_help() {
    uart_puts("do_cmd_help\r\n");
}
void do_cmd_hello() {
    uart_puts("do_cmd_hello\r\n");
}
void do_cmd_info() {
    uart_puts("do_cmd_info\r\n");
}
void do_cmd_reboot() {
    uart_puts("do_cmd_reboot\r\n");
}