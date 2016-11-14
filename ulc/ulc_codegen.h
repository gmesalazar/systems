#ifndef ulc_codegen_h
#define ulc_codegen_h

#include "ulc_vm.h"

int alloc_d();
int alloc_c();
int gen_label();
void gen_code(OpCode, long);
void back_patch(int, OpCode, long);
void dump_code();
void dump_bcodes(const char*);
void set_main_offset(int);

#endif
