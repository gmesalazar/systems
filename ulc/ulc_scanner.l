%{

#include <stdio.h>
#include <string.h>

#include "ulc_parser.h"
#include "ulc_symtable.h"
#include "../include/util.h"

#define YY_NO_INPUT

%}

%option nounput
%option yylineno

D	[0-9]
L	[a-zA-Z]

%x COMMENT

%%

[ \t\n]+            ;
"main"              {strlcpy(yylval.id, yytext, SYMNAME_MLEN); return TK_MAIN;}
"number"            {return TK_TYPE_NUM;}
"char"              {return TK_TYPE_CHR;}
";"                 {return TK_SCOLON;}
","                 {return TK_COMMA;}
"["                 {return TK_LBRACK;}
"]"                 {return TK_RBRACK;}
"("                 {return TK_LPAREN;}
")"                 {return TK_RPAREN;}
"{"                 {return TK_LBRACE;}
"}"                 {return TK_RBRACE;}
"return"            {return TK_RETURN;}
"read"              {return TK_READ;}
"write"             {return TK_WRITE;}
"if"                {return TK_IF;}
"else"              {return TK_ELSE;}
"while"             {return TK_WHILE;}
"="                 {return TK_ASSIGN;}
"?"                 {return TK_COND_IF;}
":"                 {return TK_COND_ELSE;}
"or"                {return TK_OR;}
"and"               {return TK_AND;}
"=="                {return TK_EQ;}
"!="                {return TK_NEQ;}
"<"                 {return TK_LT;}
">"                 {return TK_MT;}
"<="                {return TK_LE;}
">="                {return TK_ME;}
"+"                 {return TK_ADD;}
"-"                 {return TK_SUB;}
"*"                 {return TK_MULT;}
"/"                 {return TK_DIV;}
"%"                 {return TK_MOD;}
"!"                 {return TK_NEG;}
{D}+                {yylval.litnum = atol(yytext); return TK_LIT_NUM;}
'(\\.|[^\\'])+'     {yylval.litchar = yytext[0]; return TK_LIT_CHR;}
{L}({D}|{L})*       {strlcpy(yylval.id, yytext, SYMNAME_MLEN); return TK_NAME;}
\"(\\.|[^\\"])*\"   {yylval.litstr = strdup(yytext); return TK_LIT_STR;}
.                   {}

<INITIAL>"/*"       {BEGIN(COMMENT);}
<COMMENT>"*/"       {BEGIN(INITIAL);}
<COMMENT>[^*\n]+    {}
<COMMENT>"*"        {}
<COMMENT><<EOF>>    {return COMMENT_ERROR;}
<COMMENT>\n+        {yylineno++;}

%%
