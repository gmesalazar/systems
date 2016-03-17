%{
#include "parser.h"
#include "symtable.h"
#include "util.h"
#include <stdio.h>
#include <bsd/string.h>

#define DEBUGM DEBUG(yytext);
#define YY_NO_INPUT
%}

%option nounput

D	[0-9]
L	[a-zA-Z]

%x COMMENT

%%
[ \t\n]+		 ;
"main"		        {DEBUGM strlcpy(yylval.id, yytext, SYMNAME_MLEN); return TMAIN;}
"number"		{DEBUGM return TNUM;}
"char"			{DEBUGM return TCHAR;}
","			{DEBUGM return TCOMMA;}
";"			{DEBUGM return TSCOLON;}
"["			{DEBUGM return TLBRACK;}
"]"			{DEBUGM return TRBRACK;}
"("			{DEBUGM return TLPAREN;}
")"			{DEBUGM return TRPAREN;}
"{"			{DEBUGM return TLBRACE;}
"}"			{DEBUGM return TRBRACE;}
"return"		{DEBUGM return TRET;}
"read"			{DEBUGM return TREAD;}
"write"			{DEBUGM return TWRIT;}
"if"			{DEBUGM return TIF;}
"else"			{DEBUGM return TELSE;}
"while"			{DEBUGM return TWHILE;}
"="			{DEBUGM return TASSIGN;}
"?"			{DEBUGM return TCONDIF;}
":"			{DEBUGM return TCONDELSE;}
"or"			{DEBUGM return TOR;}
"and"			{DEBUGM return TAND;}
"=="			{DEBUGM return TEQ;}
"!="			{DEBUGM return TNEQ;}
"<"			{DEBUGM return TLT;}
">"			{DEBUGM return TMT;}
"<="			{DEBUGM return TLE;}
">="			{DEBUGM return TME;}
"+"			{DEBUGM return TSUM;}
"-"			{DEBUGM return TMIN;}
"*"			{DEBUGM return TMULT;}
"/"			{DEBUGM return TDIV;}
"%"			{DEBUGM return TMOD;}
"!"			{DEBUGM return TNOT;}
{D}+			{DEBUG("number") yylval.litnum = atol(yytext); return TNUMLIT;}
'(\\.|[^\\'])+'		{DEBUG("char literal") yylval.litchar = yytext[0]; return TCHARLIT;}
{L}({D}|{L})*		{DEBUG("identifier") strlcpy(yylval.id, yytext, SYMNAME_MLEN); return TID;}
\"(\\.|[^\\"])*\"	{DEBUG("strig literal") yylval.litstr = strdup(yytext); return TSTRLIT;}
.			 DEBUGM;

<INITIAL>"/*"		{BEGIN(COMMENT);}
<COMMENT>"*/"		{BEGIN(INITIAL);}
<COMMENT>[^*\n]+	{}
<COMMENT>"*"		{}
<COMMENT><<EOF>>	{}
<COMMENT>\n+		{yylineno++;}

%%
