#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "ulc_vm.h"

int alloc_d();
int alloc_c();
int gen_label();
void gen_code(OpCode, int);
void back_patch(int, OpCode, int);
void dump_code();
void dump_bcodes(const char*);
void set_main_offset(int);

#endif
