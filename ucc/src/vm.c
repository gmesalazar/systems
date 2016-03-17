/*
  This VM was taken (shamelessly copied) from 
  http://research.microsoft.com/en-us/um/people/rgal/ar_language/external/compiler.pdf
 */

#include <stdio.h>
#include <math.h>
#include "util.h"
#include "vm.h"
#ifdef VM
#include <stdlib.h>
#endif

const char* const op_names[] = {
    "halt", "store", "goto_false", "goto", "call", "ret",
    "data", "ld_int", "ld_var", "in_int",
    "out_int", "lt", "eq", "ne", "gt", "le", "ge", "add",
    "sub", "mult", "div", "mod", "pwr", "and", "or"
};

/* the store */
static long int section_stack[1000];
static struct instruction section_code[1000];

/* registers */
static int pc = 0;
static int ar = 0;
static int top = 0;
static struct instruction ir;

void
fetch_exec_cycle()
{
    do {
	ir = section_code[pc++];
	DEBUG(op_names[ir.op]);

	switch (ir.op) {
	case HALT:
	    break;
	case READ_INT:
	    scanf("%ld", section_stack + ar + ir.arg);
	    break;
	case WRITE_INT:
	    printf("%ld\n", section_stack[top--]);
	    break;
	case STORE:
	    section_stack[ir.arg] = section_stack[top--];
	    break;
	case GOTO_FALSE:
	    if (section_stack[top--] == 0)
		pc = ir.arg;
	    break;
	case GOTO:
	    pc = ir.arg;
	    break;
	case DATA:
	    top = top + ir.arg;
	    break;
	case LD_INT:
	    section_stack[++top] = ir.arg;
	    break;
	case LD_VAR:
	    section_stack[++top] = section_stack[ar + ir.arg];
	    break;
	case LT:
	    if (section_stack[top - 1] < section_stack[top])
		section_stack[--top] = 1;
	    else
		section_stack[--top] = 0;
	    break;
	case EQ:
	    if (section_stack[top - 1] == section_stack[top])
		section_stack[--top] = 1;
	    else
		section_stack[--top] = 0;
	    break;
	case NEQ:
	    if (section_stack[top - 1] != section_stack[top])
		section_stack[--top] = 1;
	    else
		section_stack[--top] = 0;
	    break;
	case GT:
	    if (section_stack[top - 1] > section_stack[top])
		section_stack[--top] = 1;
	    else
		section_stack[--top] = 0;
	    break;
	case LE:
	    if (section_stack[top - 1] <= section_stack[top])
		section_stack[--top] = 1;
	    else
		section_stack[--top] = 0;
	    break;
	case GE:
	    if (section_stack[top - 1] >= section_stack[top])
		section_stack[--top] = 1;
	    else
		section_stack[--top] = 0;
	    break;
	case ADD:
	    section_stack[top - 1] = section_stack[top - 1] + section_stack[top];
	    top--;
	    break;
	case SUB:
	    section_stack[top - 1] = section_stack[top - 1] - section_stack[top];
	    top--;
	    break;
	case MULT:
	    section_stack[top - 1] = section_stack[top - 1] * section_stack[top];
	    top--;
	    break;
	case DIV:
	    section_stack[top - 1] = section_stack[top - 1] * section_stack[top];
	    top--;
	    break;
	case MOD:
	    section_stack[top - 1] = section_stack[top - 1] % section_stack[top];
	    top--;
	    break;
	case PWD:
	    section_stack[top - 1] = (long) pow(section_stack[top - 1], section_stack[top]);
	    top--;
	    break;
	case AND:
	    section_stack[top - 1] = section_stack[top - 1] && section_stack[top];
	    top--;
	    break;
	case OR:
	    section_stack[top - 1] = section_stack[top - 1] || section_stack[top];
	    top--;
	    break;
	case CALL:
	    pc = ir.arg;
	    break;
	case RET:
	    pc = section_stack[top - 1];
	    section_stack[top - 1] = section_stack[top];
	    top--;
	    break;
	default:
	    printf("bad instruction: %s", op_names[ir.op]);
	}
    } while (ir.op != HALT);
}

#ifdef VM
int main (int argc, char **argv) 
{
    FILE *fin = NULL;
    struct instruction instr = {0};

    if (argc != 2)
	fatal("usage:\t%s bytecodes_file", argv[0]);
    
    fin = fopen(argv[1], "rb");
    fread(&instr, sizeof(struct instruction), 1, fin);
    while (instr.op != -1) {
	section_code[pc++] = instr;
	fread(&instr, sizeof(struct instruction), 1, fin);
    }
    /* first -1 flag: stack top*/
    top = instr.arg;
    /* segond -1 flag: main offset */
    fread(&instr, sizeof(struct instruction), 1, fin);
    pc = instr.arg;

    fclose(fin);
    fetch_exec_cycle();
    
    return EXIT_SUCCESS;
}
#endif
