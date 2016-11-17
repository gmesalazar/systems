/*
 * Environment:
 *  - Scope management
 *  - Symbols management
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ulc_environ.h"
#include "util.h"

/* pointer to the outermost scope */
static Scope *scope_head = NULL;

Scope*
/*
 * Push a new scope; called when entering
 * a scoped block
 */
push_scope(int base)
{
	Scope *new = calloc(1, sizeof(Scope));
	if (!new)
		fatal("Memory error. Compilation aborted\n");
	new->base = base; // the scope base address
	new->next_scope = scope_head; // the next visible scope
	scope_head = new;
	return new;
}

static void
pop_scope_aux(Symbol *head)
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

void
/*
 * Pop the current scope; called when
 * exiting a scoped block
 */
pop_scope()
{
	Scope *scope = scope_head;
	scope_head = scope_head->next_scope;
	pop_scope_aux(scope->symt_head); // free scope internal data
	free(scope); // free the scope itself
}

Symbol*
/*
 * Get an existing symbol by name or return
 * NULL if it does not exist
 */
get_symbol(const char *sym_name, bool recurse)
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
		if (!recurse) // check only current scope or check outer
			break;    // scopes as well?
	}
	return NULL;
}

static Symbol*
add_symbol_aux(const char *symname, Symkind kind, long addr)
{
	Symbol *ptr = calloc(1, sizeof(Symbol));
	strlcpy(ptr->name, symname, NAME_MLEN); // the symbol name
	ptr->next_symbol = scope_head->symt_head; // the next symbol
	switch(kind) { // what kind of symbol is that?
		case Sym_Data: // variable
			ptr->base = scope_head->base;
			ptr->offset = addr - scope_head->base;
			break;
		case Sym_Func: // function
			ptr->base = scope_head->base;
			ptr->offset = addr;
			break;
	}
	ptr->kind = kind;
	scope_head->symt_head = ptr;
	return ptr;
}


Symbol*
/*
 * Put a new symbol into the current scope if it does not
 * exist; only checks the current scope, that is, the new
 * symbol shadows existing symbols with the same name
 */
add_symbol(const char *symname, Symkind kind, long addr)
{
	Symbol *sym = NULL;
	if (!(sym = get_symbol(symname, false)))
		sym = add_symbol_aux(symname, kind, addr);
	else
		printf("%s is already defined\n", symname);
	return sym;
}

