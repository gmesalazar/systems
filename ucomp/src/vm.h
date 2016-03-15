#ifndef VM_H
#define VM_H

#ifndef YYDEBUG
#define YYDEBUG 0
#endif

/* op codes */
enum op_codes 
{
    HALT,
    STORE,
    GOTO_FALSE,
    GOTO,
    CALL,
    RET,
    DATA,
    LD_INT,
    LD_VAR,
    READ_INT,
    WRITE_INT,
    LT,
    EQ,
    NEQ,
    GT,
    LE,
    GE,
    ADD,
    SUB,
    MULT,
    DIV,
    MOD,
    PWD,
    AND,
    OR
};


/* instruction definition */
struct instruction
{
    enum op_codes op;
    int arg;
};

extern const char* const op_names[];

#endif
