#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ulc_symtable.h"
#include "util.h"

/* pointer to the outermost scope */
static Scope *scope_head = NULL;
static void free_table(Symbol *head);

Scope *
push_scope()
{
	Scope *new = calloc(1, sizeof(Scope));
	if (!new)
		fatal("Memory error. Compilation aborted\n");
	new->next_scope = scope_head;
	scope_head = new;
	return new;
}

void
pop_scope()
{
	Scope *aux = scope_head;
	scope_head = scope_head->next_scope;
	free_table(aux->symt_head);
	free(aux);
}


Symbol*
putsymbol(const char *symname, int data_offset)
{
	Symbol *ptr;
	ptr = calloc(1, sizeof(Symbol));
	strlcpy(ptr->name, symname, NAME_MLEN);
	ptr->next_symbol = scope_head->symt_head;
	ptr->offset = data_offset;
	scope_head->symt_head = ptr;
	return ptr;
}

Symbol*
getsymbol(const char *sym_name, int scope_level)
{
	Scope* scope_aux;
	Symbol* symt_aux;

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
	Symbol *sym = getsymbol(symname, 0);
	if (!sym)
		sym = putsymbol(symname, data_offset);
	else
		printf("%s is already defined\n", symname);
}

void
context_check(OpCode op, const char *symname)
{
	Symbol *symbol = getsymbol(symname, 1);
	if (symbol == NULL)
		printf("Undefined symbol: %s\n", symname);
	else
		gen_code(op, symbol->offset);
}

Symbol*
get_head()
{
	return scope_head->symt_head;
}

static void
free_table(Symbol *head)
{
	Symbol *aux;
	if (!head)
		return;
	while (head) {
		aux = head->next_symbol;
		free(head);
		head = aux;
	}
}
