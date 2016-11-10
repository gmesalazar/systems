#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/list.h"

int
/*
   Insert a node to a linked list

   Receive: pointers to the head of the list and to the new node
   Return: 0 on success, -1 otherwise
   */
insertNode (struct_t **head, struct_t *newNode)
{
	if (!newNode)
		return -1;

	newNode->prev = NULL;
	newNode->next = NULL;

	if (!*head) {
		*head = newNode;
	} else {
		struct_t *aux = *head;
		while (aux->next)
			aux = aux->next;
		aux->next = newNode;
		newNode->prev = aux;
	}

	return 0;
}

int
/*
   Deallocate the space used by a list

   Receive: pointer to the pointer of the list's head
   Return: 0 on success, 1 if the list was not allocated
   */
deallocStruct(struct_t **head)
{

	struct_t *aux = *head;

	if (!aux)
		return 1;

	while (aux) {
		*head = (*head)->next;
		if (aux->structType == HIST_LIST)
			deallocStruct(&aux->histData.command);
		else if (aux->structType == WORDS_LIST)
			free(aux->wordData.word);
		else if (aux->structType == TABLE) {
			free(aux->tableData.varName);
			free(aux->tableData.varValue);
		}
		free(aux);
		aux = *head;
	}
	*head = NULL;
	return 0;
}

void
/*
   Print a list, which can be a list of words (command), list of
   commands (history) or a list implementing the table of variables

   Receive: pointer to the head of the list
   */
printStruct(struct_t *head)
{

	struct_t *aux = head;
	struct_t *aux2  = NULL;
	int cnt = 1;

	while (aux) {
		switch (aux->structType) {
			case WORDS_LIST:
				printf("%s\n", aux->wordData.word);
				if (!aux->next)
					printf("\n");
				break;
			case TABLE:
				printf("%s = %s\n", aux->tableData.varName,
						aux->tableData.varValue);
				break;
			case HIST_LIST:
				aux2 = aux->histData.command;
				while (aux2) {
					printf(" %d\t%s", cnt++, aux2->wordData.word);
					aux2 = aux2->next;
				}
				printf("\n");
				break;
		}
		aux = aux->next;
	}
}

struct_t *
/*
   Return the nth element of a list

   Receive: pointer to the read of the list and position wanted
   Return: pointer to the element on success, NULL otherwise
   */
getNthNode (struct_t *head, int num)
{
	while (--num && head)
		head = head->next;

	return head;
}

struct_t *
/*
   Create a copy of a node

   Receive: pointer to the node to be cloned
   Return: pointer to the new node on success, NULL otherwise
   */
cloneNode (struct_t *node)
{
	struct_t *new = NULL;

	if ( !node )
		return NULL;

	new = (struct_t*) calloc(1, sizeof(struct_t));
	new->structType = node->structType;
	new->prev = NULL;
	new->next = NULL;

	//TODO: handle TABLE and HIST_LIST
	if (node->structType == WORDS_LIST)
		new->wordData.word = strdup(node->wordData.word);
	else
		return NULL;

	return new;
}

struct_t *
/*
   Create a copy of a list

   Receive: pointer to the head of the list to be cloned
   Return: pointer to the head of the new list on success, NULL otherwise
   */
cloneList (struct_t *head)
{
	return mergeLists (head, NULL);
}

struct_t *
/*
   Merge two lists into a new one

   Receive: pointers to the two lists' heads
   Return: pointer to the head of the new list
   */
mergeLists (struct_t *stA, struct_t *stB)
{
	struct_t *new = NULL;
	struct_t *aux = NULL;
	struct_t *node = NULL;

	if (!stA && !stB)
		return NULL;

	if (stA)
		for (aux = stA; aux; aux = aux->next) {
			node = cloneNode (aux);
			insertNode (&new, node);
		}

	if (stB)
		for (aux = stB; aux; aux = aux->next) {
			node = cloneNode (aux);
			insertNode (&new, node);
		}

	return new;
}

struct_t *
/*
   Insert a list into a given position of another list

   Receive: pointer to the head of the list to be inserted and
   pointer to the position
   */
insertListIntoPos(struct_t *listA, struct_t *pos)
{
	struct_t *start = listA;
	struct_t *end = start;

	if (!pos)
		return listA;

	while (end->next)
		end = end->next;

	start->prev = pos->prev;
	if (pos->prev)
		pos->prev->next = start;
	end->next = pos->next;
	if (pos->next)
		pos->next->prev = end;

	return start;
}
