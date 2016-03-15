#ifndef UTIL_H
#define UTIL_H

#define DEBUG(M) if (YYDEBUG) printf("%s\n", M);

void fatal(const char *fmt, ...);

#endif
