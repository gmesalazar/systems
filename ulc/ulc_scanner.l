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
"main"              {strlcpy(yylval.id, yytext, SYMNAME_MLEN); return TMAIN;}
"number"            {return TNUM;}
"char"              {return TCHAR;}
","                 {return TCOMMA;}
";"                 {return TSCOLON;}
"["                 {return TLBRACK;}
"]"                 {return TRBRACK;}
"("                 {return TLPAREN;}
")"                 {return TRPAREN;}
"{"                 {return TLBRACE;}
"}"                 {return TRBRACE;}
"return"            {return TRET;}
"read"              {return TREAD;}
"write"             {return TWRIT;}
"if"                {return TIF;}
"else"              {return TELSE;}
"while"             {return TWHILE;}
"="                 {return TASSIGN;}
"?"                 {return TCONDIF;}
":"                 {return TCONDELSE;}
"or"                {return TOR;}
"and"               {return TAND;}
"=="                {return TEQ;}
"!="                {return TNEQ;}
"<"                 {return TLT;}
">"                 {return TMT;}
"<="                {return TLE;}
">="                {return TME;}
"+"                 {return TSUM;}
"-"                 {return TMIN;}
"*"                 {return TMULT;}
"/"                 {return TDIV;}
"%"                 {return TMOD;}
"!"                 {return TNOT;}
{D}+                {yylval.litnum = atol(yytext); return TNUMLIT;}
'(\\.|[^\\'])+'     {yylval.litchar = yytext[0]; return TCHARLIT;}
{L}({D}|{L})*       {strlcpy(yylval.id, yytext, SYMNAME_MLEN); return TID;}
\"(\\.|[^\\"])*\"   {yylval.litstr = strdup(yytext); return TSTRLIT;}
.                   {}

<INITIAL>"/*"       {BEGIN(COMMENT);}
<COMMENT>"*/"       {BEGIN(INITIAL);}
<COMMENT>[^*\n]+    {}
<COMMENT>"*"        {}
<COMMENT><<EOF>>    {return COMMENT_ERROR;}
<COMMENT>\n+        {yylineno++;}

%%
