#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

#include "include/util.h"

char *
/* 
   Check if a string starts with a given string

   Return: pointer to the start of the contained string or NULL
*/
startsWith (const char *conts, const char *contd) 
{
    char *ptr = strstr(conts, contd);

    if (strlen(contd) > strlen(conts))
	return NULL;

    return ptr && (*contd == *conts) ? ptr : NULL;
}

bool
/*
  Check if a string is a valid number (i.e., can be converted to
  a number)

  Return: TRUE if the string is a valid number, FALSE otherwise
*/
isNumber (char *str) 
{
    while(*str)
	if ( !(isdigit(*str++)) )
	    return FALSE;
    return TRUE;
}

void
/* 
   Print message to stderr and exit
*/
fatal (char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    exit(-1);
}
