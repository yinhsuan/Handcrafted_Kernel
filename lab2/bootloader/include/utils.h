#ifndef _UTILS_H_
#define _UTILS_H_

#define VSPRINT_MAX_BUF_SIZE 128

unsigned int sprintf(char *dst, char *fmt, ...);
unsigned int vsprintf(char *dst, char *fmt, __builtin_va_list args);

unsigned long long strlen(const char *str);
int strcmp(const char *p1, const char *p2);
int strncmp(const char *s1, const char *s2, unsigned long long n);
char* memcpy(void *dest, const void *src, unsigned long long len);

#endif