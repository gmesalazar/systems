#ifndef CODEGEN_H
#define CODEGEN_H

#include "vm.h"

int alloc_d();
int alloc_c();
int gen_label();
void gen_code(enum op_codes op_code, int arg);
void back_patch(int addr, enum op_codes op_code, int arg);
void dump_code();
void dump_bcodes(const char *fname);
void set_main_offset(int offset);
#endif
