#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bsd/string.h>
#include "symtable.h"
#include "util.h"

/* pointer to the outermost scope */
static struct scope *scope_head = NULL;

void free_table(struct symnode *head);

struct scope *
push_scope()
{
    struct scope *new = calloc(1, sizeof(struct scope));
    if (!new)
	fatal("Memory error. Compilation aborted\n");
    new->next_scope = scope_head;
    scope_head = new;
    return new;
}

void
pop_scope()
{
    struct scope *aux = scope_head;
    scope_head = scope_head->next_scope;
    free_table(aux->symt_head);
    free(aux);
}


struct symnode*
putsymbol(const char *symname, int data_offset)
{
    struct symnode *ptr;
    ptr = calloc(1, sizeof(struct symnode));
    strlcpy(ptr->name, symname, SYMNAME_MLEN);
    ptr->next_symbol = scope_head->symt_head;
    ptr->offset = data_offset;
    scope_head->symt_head = ptr;
    return ptr;
}

struct symnode*
getsymbol(const char *sym_name, int scope_level)
{
    struct scope* scope_aux;
    struct symnode* symt_aux;

    scope_aux = scope_head;
    while (scope_aux) {
	symt_aux = scope_aux->symt_head;
	while (symt_aux) {
	    if (strcmp(symt_aux->name, sym_name) == 0)
		return symt_aux;
	    symt_aux = symt_aux->next_symbol;
	}
	scope_aux = scope_aux->next_scope;

	if (scope_level == 0)
	    break;
    }
    return NULL;
}

void
install (const char *symname, int data_offset)
{
    struct symnode *sym = getsymbol(symname, 0);
    if (!sym)
	sym = putsymbol(symname, data_offset);
    else	
	printf("%s is already defined\n", symname);
}

void
context_check(enum op_codes op_code, const char *symname)
{
    struct symnode *symbol = getsymbol(symname, 1);
    if (symbol == NULL)
	printf("Undefined symbol: %s\n", symname);
    else
	gen_code(op_code, symbol->offset);
}

struct symnode*
get_head()
{
    return scope_head->symt_head;
}

void
free_table(struct symnode *head)
{
    struct symnode *aux;
    if (!head)
	return;
    while (head) {
	aux = head->next_symbol;
	free(head);
	head = aux;
    }
}
