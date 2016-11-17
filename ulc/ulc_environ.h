#ifndef ulc_environ_h
#define ulc_environ_h

#include <stdbool.h>

#include "ulc_codegen.h"

#define NAME_MLEN 256

typedef enum symkind Symkind;

enum symkind {
	Sym_Data,
	Sym_Func
};

typedef struct symbol Symbol;

struct symbol {
	char name[NAME_MLEN];
	Symbol* next_symbol;
	int base;
	int offset;
	Symkind kind;
};

typedef struct scope Scope;

struct scope {
	Symbol *symt_head;
	Scope *next_scope;
	int base;
};

void context_check(OpCode, const char*);
Symbol* add_symbol(const char*, Symkind, long);
Symbol* get_symbol(const char*, bool);

void pop_scope();
Scope* push_scope(int base);

#endif
