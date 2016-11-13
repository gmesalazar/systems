#ifndef list_h
#define list_h

#include "uar.h"

struct node {
    struct header header;
    struct node *next;
    /* Each header is identified by a sequence number */
    unsigned int seq;
};

int list_free(void);
int list_insert(struct header);
struct node *list_lookup(const char *);
struct node *list_next(struct node *);

#endif // list_h
