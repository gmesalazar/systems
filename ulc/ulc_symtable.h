#ifndef ulc_symtable_h
#define ulc_symtable_h

#include "ulc_codegen.h"

#define NAME_MLEN 256

typedef struct symbol Symbol;

struct symbol {
	char name[NAME_MLEN];
	Symbol* next_symbol;
	long offset;
};

typedef struct scope Scope;

struct scope {
	Symbol *symt_head;
	Scope *next_scope;
};

void install(const char*, long);
void context_check(OpCode, const char*);
Symbol* getsymbol(const char*, int);
Symbol* get_head();

void pop_scope();
Scope* push_scope();

#endif
