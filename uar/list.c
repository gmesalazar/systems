/* 
   Implementation of a linked list to provide quick access to the headers 
   a given archive contains
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/list.h"
#include "include/uar.h"

static struct node *head = NULL;

int 
insert_node (struct header info) 
{
    struct node *new_node = NULL;
    static int seq;

    new_node = (struct node *) malloc(sizeof(struct node));

    if ( !new_node )
	return -1;

    new_node->header = info;
    new_node->next = NULL;

    if ( !head ) {
	head = new_node;
	seq = 1;
	head->seq = seq;
	return 0;
    } 

    else {
	struct node *aux = NULL;
	aux = head;
	while (aux->next)
	    aux = aux->next;
	aux->next = new_node;
	aux->next->seq = ++seq;
	return 0;
    }
    return 1;
}

struct node *
iterate_list (struct node *start_from) 
{
    if (!start_from)
	return head;
    return start_from->next;
}

int 
deallocate_list (void) 
{
    struct node *aux = head;
    while (aux != NULL) {
	head = head->next;
	free(aux);
	aux = head;
    }
    head = NULL;
    return 0;
}

struct node *
search_element (char *fname)
{    
    struct node * aux = head;
    while (aux && (strcmp(aux->header.fname, fname) != 0))
	aux = aux->next;
    return aux;
}
