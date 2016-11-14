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
	"HALT",
	"STORE",
	"STORE_ARG",
	"GOTO_FALSE",
	"GOTO",
	"CALL",
	"RET",
	"DATA",
	"LD_INT",
	"LD_VAR",
	"LD_AR",
	"IN_INT",
	"OUT_INT",
	"LT",
	"EQ",
	"NE",
	"GT",
	"LE",
	"GE",
	"NEG",
	"ADD",
	"SUB",
	"MULT",
	"DIV",
	"MOD",
	"PWR",
	"NOT",
	"AND",
	"OR",
	"END"
};

/* the store */
static long section_data[SEC_DATA_SZ];
static Instruction section_code[SEC_CODE_SZ];

/* registers */
static int pc = 0;      // the program counter
static int ar = 0;      // the active record
static int fp = 0;      // the frame pointer
static int sp = 0;      // the top of the stack
static Instruction ir;

void
fetch_exec_cycle()
{
	do {
		ir = section_code[pc++]; // grab our current instruction
		                         // and increment our program counter

		// what instruction is that?
		switch (ir.op) {
			case HALT:
				break;
			case READ_INT:
				// read an int into an address in the current frame
				scanf("%ld", section_data + ar + ir.arg);
				break;
			case WRITE_INT:
				// write an int which is on the top of the stack
				printf("%ld\n", section_data[sp--]);
				break;
			case STORE:
				// store the top of the stack into an address
				section_data[ir.arg] = section_data[sp--];
				break;
			case STORE_ARG:
				section_data[ir.arg] = section_data[++fp];
				break;
			case GOTO_FALSE:
				// jump to an address if the top of the stack is falsey
				if (section_data[sp--] == 0)
					pc = ir.arg;
				break;
			case GOTO:
				// jump to an address
				pc = ir.arg;
				break;
			case DATA:
				sp = sp + ir.arg;
				break;
			case LD_INT:
				// pushes an int onto the stack
				section_data[++sp] = ir.arg;
				break;
			case LD_VAR:
				// pushes the value of a variable onto the stack
				section_data[++sp] = section_data[ar + ir.arg];
				break;
			case LD_AR:
				fp = ar = sp + 1;
				break;
			case LT:
				if (section_data[sp - 1] < section_data[sp])
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
			case GT:
				if (section_data[sp - 1] > section_data[sp])
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
			case GE:
				if (section_data[sp - 1] >= section_data[sp])
					section_data[--sp] = 1;
				else
					section_data[--sp] = 0;
				break;
			case ADD:
				section_data[sp - 1] = section_data[sp - 1] + section_data[sp];
				sp--;
				break;
			case SUB:
				section_data[sp - 1] = section_data[sp - 1] - section_data[sp];
				sp--;
				break;
			case NEG:
				section_data[sp] = -section_data[sp];
				break;
			case MULT:
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
			case PWD:
				section_data[sp - 1] = (long) pow(section_data[sp - 1], section_data[sp]);
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
			case CALL:
				// set up registers for function call
				pc = ir.arg;
				break;
			case RET:
				sp = ar;
				pc = section_data[sp - 1];
				section_data[sp - 1] = section_data[sp];
				break;
			default:
				printf("bad instruction: %s", op_names[ir.op]);
		}
	} while (ir.op != HALT); // repeat until our program halts
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
	sp = instr.arg;
	// second END instruction contains the entry point pointer
	fread(&instr, sizeof(Instruction), 1, fin);
	pc = instr.arg;

	fclose(fin);
	fetch_exec_cycle();

	return EXIT_SUCCESS;
}
#endif
