#include "uart1.h"
#include "shell.h"

extern char* _bootloader_relocated_addr;
extern unsigned long long __code_size;
extern unsigned long long _start;
char* _dtb;
int relocated_flag = 1;

// Copy codeblock from _start to addr
void code_relocated(char* addr) {
    unsigned long long size = (unsigned long long)&__code_size;
    char* start = (char*)&_start;
    for (unsigned long long i=0; i<size; i++) {
        addr[i] = start[i];
    }
    ((void (*)(char*))addr)(_dtb);
}

void main (char* arg) {
    char input_buffer[CMD_MAX_LEN];
    char* relocated_ptr = (char*)&_bootloader_relocated_addr;
    _dtb = arg;

    // Relocate only once
    if (relocated_flag) {
        relocated_flag = 0;
        code_relocated(relocated_ptr);
    }

    uart_init();
    // uart_puts("loading dtb from: 0x%x\n", arg);
    cli_print_banner();

    while (1) {
        cli_cmd_clear(input_buffer, CMD_MAX_LEN);
        uart_puts("# ");
        cli_cmd_read(input_buffer);
        cli_cmd_exec(input_buffer);
    }
}