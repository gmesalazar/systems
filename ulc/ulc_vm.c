/*
 * This VM was taken shamelessly taken from (some bits were modified)
 * http://research.microsoft.com/en-us/um/people/rgal/ar_language/external/compiler.pdf
 */

#include <math.h>
#include <stdio.h>
#ifdef VM
#include <stdlib.h>
#endif

#include "ulc_vm.h"
#include "util.h"

/*
 * Opcode names array
 */
const char* const op_names[] = {
	"HLT",
	"STO",
	"JMP",
	"JMPZ",
	"CALL",
	"RET",
	"LODI",
	"LODV",
	"IN",
	"OUT",
	"LT",
	"LE",
	"GT",
	"GE",
	"EQ",
	"NEQ",
	"NEG",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"POW",
	"NOT",
	"AND",
	"OR",
};

/* the store */
static long section_data[SEC_DATA_SZ];
static Instruction section_code[SEC_CODE_SZ];

/* special purpose registers */
static int pc;      // the program counter
static int sp;      // the top of the stack
static Instruction ir;

/* general purpose registers */
static int r0;

void
fetch_exec_cycle()
{
	do {
		ir = section_code[pc++]; // grab our current instruction
		                         // and increment our program counter
		// what instruction is that?
		switch (ir.op) {
			case HLT:
				break;
			case STO:
				section_data[ir.arg1 + ir.arg2] = section_data[sp--];
				break;
			case JMP:
				pc = ir.arg2;
				break;
			case JMPZ:
				if (section_data[sp--] == 0)
					pc = ir.arg2;
				break;
			case CALL:
				pc = ir.arg2;
				break;
			case RET:
				r0 = section_data[sp--]; // save return value
				pc = section_data[sp]; // restore pc
				section_data[sp] = r0; // leave the return value
				break;
			case LODI:
				section_data[++sp] = ir.arg2;
				break;
			case LODV:
				section_data[++sp] = section_data[ir.arg1 + ir.arg2];
				break;
			case IN:
				if (ir.arg1 == -1)
					scanf("%ld", &section_data[sp]);
				else
					scanf("%ld", section_data + ir.arg1 + ir.arg2);
				break;
			case OUT:
				printf("%ld\n", section_data[sp--]);
				break;
			case LT:
				if (section_data[sp - 1] < section_data[sp])
					section_data[--sp] = 1;
				else
					section_data[--sp] = 0;
				break;
			case LE:
				if (section_data[sp - 1] <= section_data[sp])
					section_data[--sp] = 1;
				else
					section_data[--sp] = 0;
				break;
			case GT:
				if (section_data[sp - 1] > section_data[sp])
					section_data[--sp] = 1;
				else
					section_data[--sp] = 0;
				break;
			case GE:
				if (section_data[sp - 1] >= section_data[sp])
					section_data[--sp] = 1;
				else
					section_data[--sp] = 0;
				break;
			case EQ:
				if (section_data[sp - 1] == section_data[sp])
					section_data[--sp] = 1;
				else
					section_data[--sp] = 0;
				break;
			case NEQ:
				if (section_data[sp - 1] != section_data[sp])
					section_data[--sp] = 1;
				else
					section_data[--sp] = 0;
				break;
			case NEG:
				section_data[sp] = -section_data[sp];
				break;
			case ADD:
				section_data[sp - 1] = section_data[sp - 1] + section_data[sp];
				sp--;
				break;
			case SUB:
				section_data[sp - 1] = section_data[sp - 1] - section_data[sp];
				sp--;
				break;
			case MUL:
				section_data[sp - 1] = section_data[sp - 1] * section_data[sp];
				sp--;
				break;
			case DIV:
				section_data[sp - 1] = section_data[sp - 1] * section_data[sp];
				sp--;
				break;
			case MOD:
				section_data[sp - 1] = section_data[sp - 1] % section_data[sp];
				sp--;
				break;
			case POW:
				section_data[sp - 1] = pow(section_data[sp - 1], section_data[sp]);
				sp--;
				break;
			case NOT:
				section_data[sp] = !section_data[sp];
				break;
			case AND:
				section_data[sp - 1] = section_data[sp - 1] && section_data[sp];
				sp--;
				break;
			case OR:
				section_data[sp - 1] = section_data[sp - 1] || section_data[sp];
				sp--;
				break;
			default:
				printf("bad instruction: %s", op_names[ir.op]);
		}
	} while (ir.op != HLT); // repeat until our program halts
}

#ifdef VM // are we compiling the interpreter program?
int main (int argc, char **argv)
{
	FILE *fin = NULL;
	Instruction instr = {0};

	setprogname(argv[0]);

	if (argc != 2)
		fatal("usage:\t%s file", getprogname());

	if (!(fin = fopen(argv[1], "rb")))
		fatal("%s: couldn't open the bytecodes file\n", getprogname());

	// Load bytecode instructions from the file into memory
	while (true) {
		if (fread(&instr, sizeof(Instruction), 1, fin) != 1)
			fatal("%s: error loading bytecodes\n", getprogname());
		// if instruction read was "END", stop and set up stack top
		// and 'main' pointer
		if (instr.op == END)
			break;
		// ... otherwise, just increment pc and repeat
		section_code[pc++] = instr;
	}

	// first END instruction contains the stack top
	sp = instr.arg2;
	// second END instruction contains the entry point pointer
	fread(&instr, sizeof(Instruction), 1, fin);
	pc = instr.arg2;

	fclose(fin);
	fetch_exec_cycle();

	return EXIT_SUCCESS;
}
#endif
