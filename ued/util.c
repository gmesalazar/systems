#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void
err_exit(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
