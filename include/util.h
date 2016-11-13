#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

void  err_exit(const char *, ...);

char* startsWith(const char*, const char*);
bool  isNumber(char *);

ssize_t readLine(int fd, void *buff, size_t sz);
ssize_t writeLine(int sfd, const void *buff, size_t sz);

#define DEBUG(M) if (YYDEBUG) printf("%s\n", M);
void fatal(char *fmt, ...);

#endif
