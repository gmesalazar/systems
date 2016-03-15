#ifndef UTIL_H
#define UTIL_H

#define TRUE 1
#define FALSE 0

typedef unsigned char bool;

bool isNumber (char *str);
char *startsWith(const char *conts, const char *contd);
char *endsWith(const char *conts, const char *contd);
char *middledBy(const char *conts, const char *contd);
void fatal(char *fmt, ...);

#endif
