%{

#include <stdio.h>
#include <string.h>

#include "ulc_symtable.h"
#include "ulc_codegen.h"
#include "ulc_vm.h"

extern int yylex();
extern void yyerror(const char *s);

%}

%union{
    char id[100];
    char litchar;
    char *litstr;
    long int litnum;
    struct {
        int addr_goto;
        int addr_goto_false;
    } lbls;
    struct {
        char id[100];
        int addr_ret;
    } func;
}

%token <id> TMAIN
%token <id> TID
%token TNUM TCHAR
%token <litnum> TNUMLIT 
%token <litchar> TCHARLIT 
%token <litstr> TSTRLIT
%token TCOMMA TSCOLON
%token TLBRACK TRBRACK
%token TLPAREN TRPAREN
%token TLBRACE TRBRACE
%token TRET TREAD TWRIT
%token <lbls> TIF TWHILE
%token TELSE
%token TASSIGN
%token TCONDIF TCONDELSE
%token TOR TAND TEQ TNEQ
%token TLT TMT TLE TME
%token TSUM TMIN TMULT TDIV TMOD
%token TNOT

/* start symbol */
%start prog

%%

prog:
     {push_scope();} funcvardecl progdecl {gen_code(HALT, 0); pop_scope(); YYACCEPT;}
;

funcvardecl:
             type TID {install($2, alloc_d());} vardecl TSCOLON funcvardecl
           | type TID TASSIGN expr {install($2, alloc_d()); context_check(STORE, $2);} vardecl TSCOLON funcvardecl
           | type TID TLBRACK TNUMLIT TRBRACK vardecl TSCOLON funcvardecl
           | type TID TLPAREN {install($2, gen_label());} paramlist TRPAREN block funcvardecl
           |
;

progdecl:
          TMAIN {install($1, gen_label()); set_main_offset(gen_label());} block
;

vardecl:
          TCOMMA TID {install($2, alloc_d());} vardecl
        | TCOMMA TID TASSIGN expr {install($2, alloc_d()); context_check(STORE, $2);} vardecl
        | TCOMMA TID TLBRACK TNUMLIT TRBRACK vardecl
        |
;

paramlist:
           paramlistcont
         |
;

paramlistcont:
               type TID {install($2, alloc_d()); context_check(STORE, $2);}
             | type TID TCOMMA {install($2, alloc_d()); context_check(STORE, $2);} paramlistcont
             | type TID TLBRACK TRBRACK
             | type TID TLBRACK TRBRACK TCOMMA paramlistcont
;

block:
       TLBRACE {push_scope();} vardecllist commlist TRBRACE {pop_scope();}
     | TLBRACE TRBRACE
;

vardecllist:
             type TID {install($2, alloc_d());} vardecl TSCOLON vardecllist
           | type TID TASSIGN expr {install($2, alloc_d()); context_check(STORE, $2);} vardecl TSCOLON vardecllist
           | type TID TLBRACK TNUMLIT TRBRACK vardecl TSCOLON vardecllist {install($2, alloc_d());}
           |
;

type:
      TNUM
    | TCHAR
;

commlist:
          comm
        | comm commlist
;

comm:
       TSCOLON
     | expr TSCOLON
     | TRET expr TSCOLON {gen_code(RET, 0);}
     | TREAD lvalexpr {gen_code(READ_INT, $<litnum>2);} readvars TSCOLON 
     | TWRIT expr TSCOLON {gen_code(WRITE_INT, 0);}
     | TWRIT TSTRLIT TSCOLON
     | ifstmt
     | ifstmt TELSE comm {back_patch($<lbls>$.addr_goto, GOTO, gen_label());}
     | TWHILE TLPAREN {$1.addr_goto = gen_label();} expr {$1.addr_goto_false = alloc_c();} TRPAREN comm {gen_code(GOTO, $1.addr_goto);back_patch($1.addr_goto_false, GOTO_FALSE, gen_label()); }
     | block
;

readvars:
          TCOMMA lvalexpr readvars {gen_code(READ_INT, $<litnum>2);}
        |
;

ifstmt:
        TIF TLPAREN expr TRPAREN {$1.addr_goto_false = alloc_c();} comm {$1.addr_goto = alloc_c(); $<lbls>$ = $1; back_patch($1.addr_goto_false, GOTO_FALSE, gen_label()); back_patch($1.addr_goto, GOTO, gen_label());}
;

expr:
      assignexpr
;

assignexpr: condexpr
          | lvalexpr TASSIGN assignexpr {gen_code(STORE, $<litnum>1);}
;

condexpr:
           orexpr
         | orexpr TCONDIF {$<lbls>$.addr_goto_false = alloc_c();} expr {$<lbls>$.addr_goto = alloc_c(); back_patch($<lbls>3.addr_goto_false, GOTO_FALSE, gen_label());} TCONDELSE condexpr {back_patch($<lbls>5.addr_goto, GOTO, gen_label());}
;

orexpr:
        andexpr
      | orexpr TOR andexpr {gen_code(OR, 0);}
;

andexpr:
         eqexpr
       | andexpr TAND eqexpr {gen_code(AND, 0);}
;

eqexpr:
        ineqexpr
      | eqexpr TEQ ineqexpr {gen_code(EQ, 0);}
      | eqexpr TNEQ ineqexpr {gen_code(NEQ, 0);}
;

ineqexpr:
         addexpr
       | ineqexpr TLT addexpr {gen_code(LT, 0);}
       | ineqexpr TMT addexpr {gen_code(GT, 0);}
       | ineqexpr TLE addexpr {gen_code(LE, 0);}
       | ineqexpr TME addexpr {gen_code(GE, 0);}
;

addexpr:
         multexpr
       | addexpr TSUM multexpr {gen_code(ADD, 0);}
       | addexpr TMIN multexpr {gen_code(SUB, 0);}
;

multexpr:
          unexpr
        | multexpr TMULT unexpr {gen_code(MULT, 0);}
        | multexpr TDIV unexpr {gen_code(DIV, 0);}
        | multexpr TMOD unexpr {gen_code(MOD, 0);}
;

unexpr:
        primexpr
      | TMIN primexpr
      | TNOT primexpr
;

lvalexpr:
          TID {$<litnum>$ = getsymbol($1, 1)->offset;}
        | TID TLBRACK expr TRBRACK
;

primexpr:
          TID {context_check(LD_VAR, $1);}
        | TID TLPAREN {strlcpy($<func>$.id, $1, 100); $<func>$.addr_ret = alloc_c();} exprlist TRPAREN {context_check(CALL, $<func>3.id); back_patch($<func>3.addr_ret, LD_INT, gen_label());}
        | TID TLBRACK expr TRBRACK
        | TLPAREN expr TRPAREN
        | TCHARLIT
        | TNUMLIT {gen_code(LD_INT, $1);}
;

exprlist:
          assignexpr
        | exprlist TCOMMA assignexpr
        |
;

%%

void
yyerror(const char *s) 
{
    fprintf(stderr, "%s\n", s);
}
