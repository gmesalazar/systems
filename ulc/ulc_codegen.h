#ifndef ulc_codegen_h
#define ulc_codegen_h

#include "ulc_vm.h"

int alloc_data();
int alloc_code();
int label_data();
int label_code();

void gen_code(OpCode, long, long);
void back_patch(int, OpCode, long);
void prnt_code();
void save_code(const char*);
void set_main_offset(int);

#endif
