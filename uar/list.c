#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/list.h"
#include "include/uar.h"

static struct node *head = NULL;

int
list_insert(struct header info)
{
	struct node *new;

	if ((new = calloc(1, sizeof(new))) == NULL)
		return 0;

	new->header = info;
	new->next = NULL;

	if (head == NULL) {
		head = new;
		head->seq = 1;
	}
	else {
		struct node *aux = head;
		while (aux->next)
			aux = aux->next;
		aux->next = new;
		aux->next->seq++;
	}

	return 1;
}

struct node*
list_next(struct node *start)
{
	if (!start)
		return head;
	return start->next;
}

int 
list_free(void)
{
	struct node *aux;
	while ((aux = head) != NULL) {
		free(aux);
		head = head->next;
	}
	head = NULL;
	return 0;
}

struct node*
list_lookup(const char *fname)
{
	struct node *aux = head;
	while (aux && (strcmp(aux->header.fname, fname) != 0))
		aux = aux->next;
	return aux;
}
