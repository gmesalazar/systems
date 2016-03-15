#define FALSE 0
#define TRUE 1

typedef unsigned char bool;

void printHelp(int argc, char **argv);
ssize_t readLine(int fd, void *buff, size_t sz);
ssize_t writeLine(int sfd, const void *buff, size_t sz);
