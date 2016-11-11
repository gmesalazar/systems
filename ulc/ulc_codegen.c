#include <stdio.h>

#include "ulc_codegen.h"
#include "util.h"

static struct instruction section_code[1000];

static int data_offset = 0;
static int code_offset = 0;

static int main_offset = 0;

int
alloc_d()
{
	return data_offset++;
}

int
alloc_c()
{
	return code_offset++;
}

int
gen_label()
{
	return code_offset;
}

void
gen_code(OpCode op, int arg)
{
	section_code[code_offset].op = op;
	section_code[code_offset++].arg = arg;
}

void
back_patch(int addr, OpCode op, int arg)
{
	section_code[addr].op = op;
	section_code[addr].arg = arg;
}

void
dump_code()
{
	int i;
	for(i = 0; i < code_offset; i++)
		printf("%d\t%s\t%d\n", i, op_names[section_code[i].op], section_code[i].arg);
}

void
dump_bcodes(const char *fname)
{
	FILE *fd = NULL;
	if (!(fd = fopen(fname, "w")))
		fatal("Could not open bytecodes file\n");

	section_code[code_offset].op = -1;
	section_code[code_offset++].arg = data_offset;

	section_code[code_offset].op = -1;
	section_code[code_offset].arg = main_offset;

	fwrite(section_code, sizeof(struct instruction), code_offset + 1, fd);
	fclose(fd);
}

void
set_main_offset(int offset)
{
	main_offset = offset;
}
