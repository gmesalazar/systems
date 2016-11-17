#ifndef util_h
#define util_h

#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

void  err_exit(const char *, ...);

char* startsWith(const char*, const char*);
bool  isNumber(char *);

ssize_t readLine(int fd, void *buff, size_t sz);
ssize_t writeLine(int sfd, const void *buff, size_t sz);

void fatal(char *fmt, ...);

#endif
