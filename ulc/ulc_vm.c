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
	"GOTO_FALSE",
	"GOTO",
	"CALL",
	"RET",
	"DATA",
	"LD_INT",
	"LD_VAR",
	"IN_INT",
	"OUT_INT",
	"LT",
	"EQ",
	"NE",
	"GT",
	"LE",
	"GE",
	"ADD",
	"SUB",
	"MULT",
	"DIV",
	"MOD",
	"PWR",
	"AND",
	"OR",
	"END"
};

/* the store */
static long section_data[1000];
static Instruction section_code[1000];

/* registers */
static int pc = 0;      // the program counter
static int ar = 0;      // the active record
static int sp = 0;      // the top of the stack
static Instruction ir;

void
fetch_exec_cycle()
{
	do {
		ir = section_code[pc++]; // grab our current instruction
		                         // and increment our program counter
		DEBUG(op_names[ir.op]);

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
				pc = section_data[sp - 1];
				section_data[sp - 1] = section_data[sp];
				sp--;
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
