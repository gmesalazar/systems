#ifndef ulc_vm_h
#define ulc_vm_h

#define SEC_CODE_SZ 2048
#define SEC_DATA_SZ 4096

/*
 * Opcodes
 */
typedef enum {
	HLT,  // HLT,   0,      0
	STO,  // STO,   BASE, OFF: store the top of the stack in BASE+OFF
	JMP,  // JMP,   0,    NPC: set PC to NPC
	JMPZ, // JMPZ,  0,    NPC: set PC to NPC if top of stack is zero
	CALL, // CALL,  NFP,  NPC: set FP to NFP and FP to NFP
	RET,  // RET,   0,      0: restore PC and leave value on the stack
	LODI, // LODI,  0,    VAL: load integer onto the stack
	LODV, // LODV,  BASE, OFF: load value at BASE+OFF onto the stack
	IN,   // IN,    BASE, OFF: read standard input into BASE+OFF
	OUT,  // OUT,   0,      0: write the stack top to standard out
	LT,   // LT,    0,      0: STACK[TOP-1] =  STACK[TOP-1] <  STACK[TOP]; TOP--
	LE,   // LE,    0,      0: STACK[TOP-1] =  STACK[TOP-1] <= STACK[TOP]; TOP--
	GT,   // GT,    0,      0: STACK[TOP-1] =  STACK[TOP-1] >  STACK[TOP]; TOP--
	GE,   // GE,    0,      0: STACK[TOP-1] =  STACK[TOP-1] >= STACK[TOP]; TOP--
	EQ,   // EQ,    0,      0: STACK[TOP-1] =  STACK[TOP-1] == STACK[TOP]; TOP--
	NEQ,  // NEQ    0,      0: STACK[TOP-1] =  STACK[TOP-1] != STACK[TOP]; TOP--
	NEG,  // NEG    0,      0: STACK[TOP]   = -STACK[TOP]
	ADD,  // ADD    0,      0: STACK[TOP-1] =  STACK[TOP-1] +  STACK[TOP]; TOP--
	SUB,  // SUB    0,      0: STACK[TOP-1] =  STACK[TOP-1] -  STACK[TOP]; TOP--
	MUL,  // MUL    0,      0: STACK[TOP-1] =  STACK[TOP-1] *  STACK[TOP]; TOP--
	DIV,  // DIV    0,      0: STACK[TOP-1] =  STACK[TOP-1] /  STACK[TOP]; TOP--
	MOD,  // MOD    0,      0: STACK[TOP-1] =  STACK[TOP-1] %  STACK[TOP]; TOP--
	POW,  // POW    0,      0: STACK[TOP-1] =  STACK[TOP-1] ^  STACK[TOP]; TOP--
	NOT,  // NOT    0,      0: STACK[TOP]   = !STACK[TOP]
	AND,  // AND    0,      0: STACK[TOP-1] =  STACK[TOP-1] && STACK[TOP]; TOP--
	OR,   // OR     0,      0: STACK[TOP-1] =  STACK[TOP-1] || STACK[TOP]; TOP--
	END   // placeholder
} OpCode;

/*
 * Operation names array:
 * Index it with the opcode and get the name
 */
extern const char* const op_names[];

/* instruction definition */
typedef struct instruction {
	OpCode op;
	long arg1;
	long arg2;
} Instruction;

#endif
