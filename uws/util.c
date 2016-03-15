#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "include/util.h"

ssize_t
/* Ref: http://man7.org/tlpi/code/online/dist/sockets/read_line.c.html */
readLine(int fd, void *buffer, size_t sz)
{
    ssize_t numRead;
    size_t totRead;
    char *buff;
    char ch;

    if (sz <= 0 || buffer == NULL) {
	errno = EINVAL;
	return -1;
    }

    buff = buffer;
    totRead = 0;

    while(TRUE) {
	numRead = read(fd, &ch, 1);
	
	if (numRead == -1) {
	    if (errno == EINTR)
		continue;
	    else
		return -1;
	}

	else if (numRead == 0) {
	    if (totRead == 0)
		return 0;
	    else
		break;
	}

	else {
	    if (totRead < sz - 1) {
		totRead++;
		*buff++ = ch;
	    }
	    if (ch == '\n')
		break;
	}
    }
    *buff = '\0';
    return totRead;
}

ssize_t
/* Ref: http://www.paulgriffiths.net/program/c/srcs/echoservsrc.html */
writeLine(int sockd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *buffer;

    buffer = vptr;
    nleft  = n;

    while ( nleft > 0 ) {
	if ( (nwritten = write(sockd, buffer, nleft)) <= 0 ) {
	    if ( errno == EINTR )
		nwritten = 0;
	    else;
	}
	nleft  -= nwritten;
	buffer += nwritten;
    }

    return n;
}

void
printHelp(int argc, char **argv) 
{
    fprintf(stdout, "USAGE: %s <options>\n", argv[0]);
    fprintf(stdout, "\n<options>\n");
    fprintf(stdout, "-start: start the webserver\n-verbose: activate verbose mode\n");
}
