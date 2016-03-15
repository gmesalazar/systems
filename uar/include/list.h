#ifndef LIST_H
#define LIST_H

#include "uar.h"

struct node {
    struct header header;
    struct node *next;
    /* Each header is identified by a sequence number */
    unsigned int seq;
};

/* Receive a header object and insert it into the list of headers */
extern int insert_node (struct header info);
/* Iterate over the list of headers -- return  NULL when upon reaching the end */
extern struct node *iterate_list (struct node *start_from);
extern int deallocate_list (void);
/* Search for a header using the name as key; return NULL if not found */
extern struct node *search_element (char *fname);

#endif //LIST_H
