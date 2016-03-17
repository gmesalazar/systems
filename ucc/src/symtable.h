#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "codegen.h"
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

void install(const char *symname, int data_offset);
void context_check(enum op_codes opcode, const char *symname);
struct symnode* getsymbol(const char *symname, int scope_level);
struct symnode* get_head();

void
pop_scope();

struct scope*
push_scope();

#endif
