#ifndef ulc_object_h
#define ulc_object_h

#include "ulc_environ.h"

typedef struct function {
	char* id;
	long  ret;
	int nl; // number of locals
} TFunction;

typedef struct jmplabel {
	long addr_goto;
	long addr_goto_false;
} TJmpLabel;

typedef enum type {
	TYPE_NUMBER,
	TYPE_STRING
} TType;

typedef struct value {
	long val;
	TType type;
} TValue;

#endif
