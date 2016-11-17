#include <stdio.h>

#include "ulc_codegen.h"
#include "ulc_vm.h"
#include "util.h"

/* memory to hold code while it's generated */
static struct instruction section_code[SEC_CODE_SZ];

/* keep track of data area allocation */
static int data_offset = 0;
/* keep track of code area allocation */
static int code_offset = 0;
/* keep track of the entrypoint's offset */
static int main_offset = 0;

int
alloc_data()
{
	return data_offset++;
}

int
alloc_code()
{
	return code_offset++;
}

int
label_code()
{
	return code_offset;
}

int
label_data()
{
	return data_offset;
}

void
gen_code(OpCode op, long arg1, long arg2)
{
	section_code[code_offset].op = op;
	section_code[code_offset].arg1 = arg1;
	section_code[code_offset++].arg2 = arg2;
}

void
back_patch(int addr, OpCode op, long arg)
{
	section_code[addr].op = op;
	section_code[addr].arg2 = arg;
}

void
dump_code()
{
	int i;
	for(i = 0; i < code_offset; i++)
		printf("%d\t%s\t%ld %ld\n", i, op_names[section_code[i].op],
				section_code[i].arg1, section_code[i].arg2);
}

void
dump_bcodes(const char *fname)
{
	FILE *fd = NULL;
	if (!(fd = fopen(fname, "w")))
		fatal("Could not open bytecodes file\n");

	section_code[code_offset].op = END;
	section_code[code_offset].arg1 = 0;
	section_code[code_offset++].arg2 = data_offset;

	section_code[code_offset].op = END;
	section_code[code_offset].arg1 = 0;
	section_code[code_offset].arg2 = main_offset;

	fwrite(section_code, sizeof(Instruction), code_offset + 1, fd);
	fclose(fd);
}

void
set_main_offset(int offset)
{
	main_offset = offset;
}
