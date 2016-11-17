%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ulc_codegen.h"
#include "ulc_environ.h"
#include "ulc_object.h"
#include "ulc_vm.h"
#include "util.h"

extern int yylex();
extern void yyerror(const char *s);
extern int yylineno;

#define check_gen_code(opcode, name) {                        \
        Symbol *s = NULL;                                     \
        if(!(s = get_symbol(name, true)))                     \
            fprintf(stderr, "Undefined symbol: %s\n", name);  \
        else                                                  \
            gen_code(opcode, s->offset);}

%}

%union{
    char id[100];
    long litnum;
    TType     type;
    TValue    litv;
    TJmpLabel lbls;
    TFunction func;
}

%type <type> type

%token <id>     TK_MAIN
%token <id>     TK_NAME
%token <type>   TK_TYPE_NUM TK_TYPE_STR
%token <litnum> TK_LIT_NUM
%token <litnum> TK_LIT_STR
%token TK_COMMA TK_SCOLON
%token TK_LBRACK TK_RBRACK
%token TK_LPAREN TK_RPAREN
%token TK_LBRACE TK_RBRACE
%token TK_RETURN TK_READ TK_WRITE
%token <lbls> TK_IF TK_WHILE
%token TK_ELSE
%token TK_ASSIGN
%token TK_COND_IF TK_COND_ELSE
%token TK_OR TK_AND TK_EQ TK_NEQ
%token TK_LT TK_MT TK_LE TK_ME
%token TK_ADD TK_SUB TK_MULT TK_DIV TK_MOD
%token TK_NEG

/* flag token to signal unterminated comments from the scanner */
%token COMMENT_ERROR;

/* start symbol */
%start prog

%%

prog:
     {push_scope();} funcvardecl progdecl {gen_code(HALT, 0); pop_scope(); YYACCEPT;}
;

funcvardecl:
             type TK_NAME {add_symbol($2, alloc_d());} vardecl TK_SCOLON funcvardecl
           | type TK_NAME TK_ASSIGN expr {add_symbol($2, alloc_d()); check_gen_code(STORE, $2);} vardecl TK_SCOLON funcvardecl
           | type TK_NAME TK_LBRACK TK_LIT_NUM TK_RBRACK vardecl TK_SCOLON funcvardecl
           | type TK_NAME TK_LPAREN {add_symbol($2, gen_label());} paramlist TK_RPAREN block funcvardecl
           |
;

progdecl:
          TK_MAIN {add_symbol($1, gen_label()); set_main_offset(gen_label());} block
;

vardecl:
          TK_COMMA TK_NAME {add_symbol($2, alloc_d());} vardecl
        | TK_COMMA TK_NAME TK_ASSIGN expr {add_symbol($2, alloc_d()); check_gen_code(STORE, $2);} vardecl
        | TK_COMMA TK_NAME TK_LBRACK TK_LIT_NUM TK_RBRACK vardecl
        |
;

paramlist:
           paramlistcont
         |
;

paramlistcont:
               type TK_NAME {add_symbol($2, alloc_d()); check_gen_code(STORE_ARG, $2);}
             | type TK_NAME TK_COMMA {add_symbol($2, alloc_d()); check_gen_code(STORE_ARG, $2);} paramlistcont
             | type TK_NAME TK_LBRACK TK_RBRACK
             | type TK_NAME TK_LBRACK TK_RBRACK TK_COMMA paramlistcont
;

block:
       TK_LBRACE {push_scope();} vardecllist commlist TK_RBRACE {pop_scope();}
     | TK_LBRACE TK_RBRACE
;

vardecllist:
             type TK_NAME {add_symbol($2, alloc_d());} vardecl TK_SCOLON vardecllist
           | type TK_NAME TK_ASSIGN expr {add_symbol($2, alloc_d()); check_gen_code(STORE, $2);} vardecl TK_SCOLON vardecllist
           | type TK_NAME TK_LBRACK TK_LIT_NUM TK_RBRACK vardecl TK_SCOLON vardecllist {add_symbol($2, alloc_d());}
           |
;

type:
      TK_TYPE_NUM {$$ = TYPE_NUMBER;}
    | TK_TYPE_STR {$$ = TYPE_STRING;}
;

commlist:
          comm
        | comm commlist
;

comm:
       TK_SCOLON
     | expr TK_SCOLON
     | TK_RETURN expr TK_SCOLON {gen_code(RET, 0);}
     | TK_READ lvalexpr {gen_code(READ_INT, $<litnum>2);} readvars TK_SCOLON 
     | TK_WRITE expr TK_SCOLON {gen_code(WRITE_INT, 0);}
     | TK_WRITE TK_LIT_STR TK_SCOLON
     | ifstmt
     | ifstmt TK_ELSE comm {back_patch($<lbls>$.addr_goto, GOTO, gen_label());}
     | TK_WHILE TK_LPAREN {$1.addr_goto = gen_label();} expr {$1.addr_goto_false = alloc_c();} TK_RPAREN comm {gen_code(GOTO, $1.addr_goto);back_patch($1.addr_goto_false, GOTO_FALSE, gen_label()); }
     | block
;

readvars:
          TK_COMMA lvalexpr readvars {gen_code(READ_INT, $<litnum>2);}
        |
;

ifstmt:
        TK_IF TK_LPAREN expr TK_RPAREN {$1.addr_goto_false = alloc_c();} comm {$1.addr_goto = alloc_c(); $<lbls>$ = $1; back_patch($1.addr_goto_false, GOTO_FALSE, gen_label()); back_patch($1.addr_goto, GOTO, gen_label());}
;

expr:
      assignexpr
;

assignexpr: condexpr
          | lvalexpr TK_ASSIGN assignexpr {gen_code(STORE, $<litnum>1);}
;

condexpr:
           orexpr
         | orexpr TK_COND_IF {$<lbls>$.addr_goto_false = alloc_c();} expr {$<lbls>$.addr_goto = alloc_c(); back_patch($<lbls>3.addr_goto_false, GOTO_FALSE, gen_label());} TK_COND_ELSE condexpr {back_patch($<lbls>5.addr_goto, GOTO, gen_label());}
;

orexpr:
        andexpr
      | orexpr TK_OR andexpr {gen_code(OR, 0);}
;

andexpr:
         eqexpr
       | andexpr TK_AND eqexpr {gen_code(AND, 0);}
;

eqexpr:
        ineqexpr
      | eqexpr TK_EQ ineqexpr {gen_code(EQ, 0);}
      | eqexpr TK_NEQ ineqexpr {gen_code(NEQ, 0);}
;

ineqexpr:
         addexpr
       | ineqexpr TK_LT addexpr {gen_code(LT, 0);}
       | ineqexpr TK_MT addexpr {gen_code(GT, 0);}
       | ineqexpr TK_LE addexpr {gen_code(LE, 0);}
       | ineqexpr TK_ME addexpr {gen_code(GE, 0);}
;

addexpr:
         multexpr
       | addexpr TK_ADD multexpr {gen_code(ADD, 0);}
       | addexpr TK_SUB multexpr {gen_code(SUB, 0);}
;

multexpr:
          unexpr
        | multexpr TK_MULT unexpr {gen_code(MULT, 0);}
        | multexpr TK_DIV unexpr {gen_code(DIV, 0);}
        | multexpr TK_MOD unexpr {gen_code(MOD, 0);}
;

unexpr:
        primexpr
      | TK_SUB primexpr {gen_code(NEG, 0);}
      | TK_NEG primexpr {gen_code(NOT, 0);}
;

lvalexpr:
          TK_NAME {$<litnum>$ = get_symbol($1, 1)->offset;}
        | TK_NAME TK_LBRACK expr TK_RBRACK
;

primexpr:
          TK_NAME {check_gen_code(LD_VAR, $1);}
        | TK_NAME TK_LPAREN {strlcpy($<func>$.id, $1, 100); $<func>$.ret = alloc_c(); gen_code(LD_AR, 0);} exprlist TK_RPAREN {check_gen_code(CALL, $<func>3.id); back_patch($<func>3.ret, LD_INT, gen_label());}
        | TK_NAME TK_LBRACK expr TK_RBRACK
        | TK_LPAREN expr TK_RPAREN
        | TK_LIT_NUM {gen_code(LD_INT, $1);}
        | TK_LIT_STR {gen_code(LD_INT, (long)strdup((char *) $1));}
;

exprlist:
          assignexpr
        | exprlist TK_COMMA assignexpr
        |
;

%%

void
yyerror(const char *s) 
{
    switch(yychar) {
        case COMMENT_ERROR:
            fprintf(stderr, "%s:" KRED " error" KNRM " at line %d: unterminated comment\n",
                getprogname(), yylineno);
            yychar = YYEOF;
            break;
        default:
            fprintf(stderr, "%s:" KRED " error" KNRM " at line %d: %s\n",
                getprogname(), yylineno, s);
            break;
    }
}
