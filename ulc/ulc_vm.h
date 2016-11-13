#ifndef VM_H
#define VM_H

#ifndef YYDEBUG
#define YYDEBUG 0
#endif

/*
 * Opcodes
 */
typedef enum {
	HALT,
	STORE,
	STORE_ARG,
	GOTO_FALSE,
	GOTO,
	CALL,
	RET,
	DATA,
	LD_INT,
	LD_VAR,
	LD_AR,
	READ_INT,
	WRITE_INT,
	LT,
	EQ,
	NEQ,
	GT,
	LE,
	GE,
	NEG,
	ADD,
	SUB,
	MULT,
	DIV,
	MOD,
	PWD,
	NOT,
	AND,
	OR,
	END
} OpCode;

/*
 * Operation names array:
 * Index it with the opcode and get the name
 */
extern const char* const op_names[];

/* instruction definition */
typedef struct instruction {
	OpCode op;
	int arg;
} Instruction;

#endif
