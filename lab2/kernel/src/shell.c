#include <stddef.h>
#include "shell.h"
#include "uart1.h"
#include "mbox.h"
#include "power.h"
#include "cpio.h"
#include "utils.h"
#include "dtb.h"

extern char* dtb_ptr;
void* CPIO_DEFAULT_PLACE;

struct CLI_CMDS cmd_list[CLI_MAX_CMD] = {
    {.command="hello", .help="print Hello World!"},
    {.command="help", .help="print the help menu"},
    {.command="info", .help="get device information via mailbox"},
    {.command="ls", .help="list directory contents"},
    {.command="reboot", .help="reboot the device"}
};

void cli_print_banner() {
    uart_puts("=======================================\r\n");
    uart_puts("  Welcome to NYCU-OSC 2023 Lab2 Shell  \r\n");
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
    if (!buffer) return;

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
    else if (cli_cmd_strcmp(buffer, "ls") == 0) {
        do_cmd_ls();
    }
    else if (*buffer) {
        uart_puts(buffer);
        uart_puts(": command not found\r\n");
    }
}

void do_cmd_help() {
    for (int i=0; i<CLI_MAX_CMD; i++) {
        uart_puts(cmd_list[i].command);
        uart_puts("\t\t: ");
        uart_puts(cmd_list[i].help);
        uart_puts("\r\n");
    }
}

void do_cmd_hello() {
    uart_puts("Hello World!\r\n");
}

void do_cmd_info() {
    // print hw revision
    pt[0] = 8 * 4; // the size of the mailbox message in bytes
    pt[1] = MBOX_REQUEST_PROCESS; // specify the type of message being sent -> indicate that the mailbox message is a request that should be processed by the receiver
    pt[2] = MBOX_TAG_GET_BOARD_REVISION; // the specific information that is being requested
    pt[3] = 4; // the length of the data that is being requested
    // specify a request code associated with the message being sent
    // -> This request code is used by the GPU to "determine the type of request" being made and the action to be taken in response
    // -> "no additional request code is associated with the mailbox message being sent"
    pt[4] = MBOX_TAG_REQUEST_CODE;
    pt[5] = 0;
    pt[6] = 0;
    pt[7] = MBOX_TAG_LAST_BYTE;

    // send the message to the GPU and retrieve the requested information
    if (mbox_call(MBOX_TAGS_ARM_TO_VC, (unsigned int)((unsigned long)&pt))) {
        uart_puts("Hardware Revision\t: ");
        uart_2hex(pt[6]);
        uart_2hex(pt[5]);
        uart_puts("\r\n");
    }
    // print arm memory
    pt[0] = 8 * 4;
    pt[1] = MBOX_REQUEST_PROCESS;
    pt[2] = MBOX_TAG_GET_ARM_MEMORY;
    pt[3] = 8; // the response buffer should be 8 bytes in length
    pt[4] = MBOX_TAG_REQUEST_CODE;
    pt[5] = 0;
    pt[6] = 0;
    pt[7] = MBOX_TAG_LAST_BYTE;

    if (mbox_call(MBOX_TAGS_ARM_TO_VC, (unsigned int)((unsigned long)&pt))) {
        uart_puts("ARM Memory Base Address\t: ");
        uart_2hex(pt[5]);
        uart_puts("\r\n");
        uart_puts("ARM Memory Size\t\t: ");
        uart_2hex(pt[6]);
        uart_puts("\r\n");
    }
}

void do_cmd_reboot() {
    uart_puts("Reboot in 5 seconds ...\r\n\r\n");

    volatile unsigned int* rstc_addr = (unsigned int*) PM_RSTC;
    *rstc_addr = PM_PASSWORD | 0x20;

    volatile unsigned int* wdog_addr = (unsigned int*) PM_WDOG;
    *wdog_addr = PM_PASSWORD | 5;
}

void do_cmd_ls() {
    // uart_puts("Enter do_cmd_ls\r\n\r\n");

    char* c_filepath;
    char* c_filedata;
    unsigned int c_filesize;
    struct cpio_newc_header *header_ptr = CPIO_DEFAULT_PLACE;

    while (header_ptr != 0) {
        int error = cpio_newc_parse_header(header_ptr, &c_filepath, &c_filesize, &c_filedata, &header_ptr);

        if (error) {
            uart_puts("cpio parse error!!!");
            break;
        }
        //if this is not TRAILER!!! (last of file)
        if (header_ptr != 0) {
            uart_puts("%s\n", c_filepath);
        }
    }
}