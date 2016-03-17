#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void
/* 
   Print message to stderr and exit
*/
fatal (char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
