#ifndef uws_h
#define uws_h

#define PORT_NUM "8080"
#define IN_QUEUE_SZ 7

#define PACKET_BUFF_SZ 1024
#define PAGES_DIR "pages"

const char *found_header[] =
{"HTTP/1.1 200 OK\r\n",
 "Content-Type: text/html\r\n",
 "\r\n"};

const char *not_found_header[] = 
{"HTTP/1.1 404 Not Found\r\n",
 "Content-Type: text/html\r\n",
 "\r\n"};

#endif // uws_h
