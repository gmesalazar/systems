#ifndef ulc_symtable_h
#define ulc_symtable_h

#include "ulc_codegen.h"

#define SYMNAME_MLEN 100

struct scope {
	struct symnode *symt_head;
	struct scope *next_scope;
};

struct symnode
{
	char name[SYMNAME_MLEN];
	int offset;
	struct symnode* next_symbol;
};

void install(const char*, int);
void context_check(OpCode, const char*);
struct symnode* getsymbol(const char*, int);
struct symnode* get_head();

void pop_scope();
struct scope* push_scope();

#endif
