#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "include/uws.h"
#include "util.h"

static void startServer(void);
static void parseArgs(int argc, char **argv);
static void doNetworkingStuff(void);
static void logSockInfo(int sfd, int inOut);
static void readRequest(int sfd);
static void processRequest(int sfd, char *buff, ssize_t sz);
static void processGETReq(int sfd, char **reqLine);
static void *threadCallback(void *sfd);
static void writeHeader(int sfd, const char *header[], int sz);
static void printHelp(int argc, char **argv);

static bool verbose = false;

int 
main(int argc, char **argv) 
{
    parseArgs(argc, argv);
    return EXIT_SUCCESS;
}

static void
parseArgs(int argc, char **argv)
{
    int i;

    if (argc < 2) {
	printHelp(argc, argv);
	exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++)
	if (strcmp(argv[i], "-verbose") == 0)
	    verbose = false;

    for (i = 1; i < argc; i++)
	if (strcmp(argv[i], "-start") == 0)
	    startServer();
}

static void
startServer(void) 
{
    if (chdir(PAGES_DIR) != 0)
	fprintf(stdout, "chdir error: %s\n", strerror(errno));
    doNetworkingStuff();
}

static void
doNetworkingStuff(void)
{
    struct sockaddr_storage peerAddr;
    struct addrinfo hints, *res, *aux;
    int connections[IN_QUEUE_SZ];
    int bstatus = -1;
    int ssocket = -1;
    socklen_t addrSize;
    int port = 8080, i = 0;;
    char portstr[20] = {0};
    pthread_t thread;    
    pthread_attr_t attr;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    snprintf(portstr, 20, "%d", port++);

    if (getaddrinfo(NULL, portstr, &hints, &res) != 0) {
	fprintf(stdout, "getaddrinfo error: %s\n", strerror(errno));
	goto cleanup;
    }

    aux = res;
    while (aux) {
	if ((ssocket = socket(aux->ai_family, aux->ai_socktype, aux->ai_protocol)) == -1)
	    continue;
	if ((bstatus = bind(ssocket, aux->ai_addr, aux->ai_addrlen)) == 0) {
	    aux = aux->ai_next;
	    break;
	} else if (bstatus == -1) {
	    if (errno == EADDRINUSE) {
		snprintf(portstr, 20, "%d", port++);
		freeaddrinfo(res);
		getaddrinfo(NULL, portstr, &hints, &res);
		aux = res;
	    }
	}
    }

    freeaddrinfo(res);

    if (bstatus == -1) {
	fprintf(stdout, "bind error: %s\n", strerror(errno));
	goto cleanup;
    }

    if ((listen(ssocket, IN_QUEUE_SZ)) == -1) {
	fprintf(stdout, "listen error: %s\n", strerror(errno));
	goto cleanup;
    }

    addrSize = sizeof(struct sockaddr_storage);

    if (verbose)
	logSockInfo(ssocket, 0);
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    while (true) {
    
	connections[i] = accept(ssocket, (struct sockaddr*) &peerAddr, &addrSize);

	if (verbose)
	    logSockInfo(connections[i], 1);
       
	if (pthread_create(&thread, &attr, threadCallback, (void*) &connections[i]) !=0)
	    continue;

	i = (i + 1) % IN_QUEUE_SZ;
    }

    close(ssocket);

  cleanup:
    /* I'm sorry, Dr. Dijkstra */
    ;
}

static void
*threadCallback(void* sfd)
{
    int csocket = *(int*) sfd;
    readRequest(csocket);
    close(csocket);
    pthread_exit(NULL);
}

static void
readRequest(int sfd) 
{
    char *buff = NULL;
    ssize_t nread;

    buff = (char *) calloc((long)PACKET_BUFF_SZ, sizeof(char));
	
    if (!buff)
	return;
	
    /* read request line */
    nread = readLine(sfd, buff, (long) PACKET_BUFF_SZ);

    /* ignore request headers -- we don't need them :) */
    while(true) {
	char tmp[PACKET_BUFF_SZ] = {'\0'};
	readLine(sfd, tmp, (long) PACKET_BUFF_SZ);
	if (strcmp(tmp, "\r\n") == 0)
	    break;
    }
    processRequest(sfd, buff, nread);

    free(buff);
}

static void
processRequest(int sfd, char *buff, ssize_t sz)
{
    char *reqLine[3];
    char *aux;
    
    aux = strtok(buff, " ");
    reqLine[0] = aux;
    aux = strtok(NULL, " ");
    reqLine[1] = aux;
    aux = strtok(NULL, " ");
    reqLine[2] = aux;
   
    if (strcmp(reqLine[0], "GET") == 0)
	processGETReq(sfd, reqLine);
    else
	;
}

static void
processGETReq(int sfd, char **reqLine)
{
    FILE *fp = NULL;
    char ch;

    if (strlen(reqLine[1]) == 1)
	reqLine[1] = "/index.html";

    if ((fp = fopen(reqLine[1] + 1, "r"))) {
	
	writeHeader(sfd, found_header, sizeof(found_header)/sizeof(char*));
	while ((ch = fgetc(fp)) != EOF)
	    write(sfd, &ch, 1);

	fclose(fp);
    }

    else {

	if (!(fp = fopen("errors/404.html", "r")))
	    return;

	writeHeader(sfd, not_found_header, sizeof(not_found_header)/sizeof(char*));
	while ((ch = fgetc(fp)) != EOF)
	    write(sfd, &ch, 1);

	fclose(fp);
    }
}


static void
writeHeader(int sfd, const char *header[], int sz)
{
    int i;
    for (i = 0; i < sz; i++)
	writeLine(sfd, header[i], strlen(header[i]));
}

static void
logSockInfo(int sock, int inOut)
{
    socklen_t size;
    struct sockaddr addr;
    struct sockaddr_in *addrin;
    char addrPrint[40];

    size = sizeof(addr);

    if (inOut == 0)
	getsockname(sock, &addr, &size);
    else if (inOut == 1)
	getpeername(sock, &addr, &size);

    addrin = (struct sockaddr_in*) &addr;
    inet_ntop(addrin->sin_family, &(addrin->sin_addr), addrPrint, 40);

    if (inOut == 0)
	printf("Listening on %s:%d\n", addrPrint, 
	       ntohs(addrin->sin_port));
    else if (inOut == 1)
	printf("Accepted from %s:%d\n", addrPrint, 
	       ntohs(addrin->sin_port));

    fflush(stdout);
}

void
printHelp(int argc, char **argv) 
{
    fprintf(stdout, "USAGE: %s <options>\n", argv[0]);
    fprintf(stdout, "\n<options>\n");
    fprintf(stdout, "-start: start the webserver\n-verbose: activate verbose mode\n");
}
