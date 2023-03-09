#ifndef _SHELL_H_
#define _SHELL_H_

#define CMD_MAX_LEN 32

void cli_print_banner();

void cli_cmd_clear(char*, int);
void cli_cmd_read(char*);
void cli_cmd_exec(char*);
int cli_cmd_strcmp(const char*, const char*);

void do_cmd_help();
void do_cmd_hello();
void do_cmd_info();
void do_cmd_reboot();

#endif