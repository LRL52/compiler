%{
#include "token.h"
%}

WHITE_SPACE	[ \t\n\r]+

%%

"+"		{return Y_ADD;}
"-" 	{return Y_SUB;}
"*" 	{return Y_MUL;}
"/" 	{return Y_DIV;}
"&&"	{return Y_AND;}
"||"	{return Y_OR;}
"%"		{return Y_MODULO;}
"="		{return Y_ASSIGN;}

"int"		{return Y_INT;}
"float"		{return Y_FLOAT;}
"double"	{return Y_FLOAT;}
"if"		{return Y_IF;}
"else"		{return Y_ELSE;}
"while"		{return Y_WHILE;}
"void"		{return Y_VOID;}
"const"		{return Y_CONST;}
"break"		{return Y_BREAK;}
"continue"	{return Y_CONTINUE;}
"return"	{return Y_RETURN;}

"<"		{return Y_LESS;}
">"		{return Y_GREAT;}
"<="	{return Y_LESSEQ;}
">="	{return Y_GREATEQ;}
"=="	{return Y_EQ;}
"!="	{return Y_NOTEQ;}
"!"		{return Y_NOT;}
"<<"	{return Y_LPAR;}
">>"	{return Y_RPAR;}
","		{return Y_COMMA;}
"("		{return Y_LPAR;}
")"		{return Y_RPAR;}
"["		{return Y_LSQUARE;}
"]"		{return Y_RSQUARE;}
"{"		{return Y_LBRACKET;}
"}"		{return Y_RBRACKET;}
";"		{return Y_SEMICOLON;}

{WHITE_SPACE}	{return 1;}
[0-9]+  {return num_INT;}
[0-9]+\.[0-9]+ {return num_FLOAT;}
[a-zA-Z_][a-zA-Z0-9_]*	{return Y_ID;}

.	{printf("Error: 404 Not Found\n"); return 404;}

%%

int main(int argc, char **argv)
{
	int tok;

	while(tok = yylex()) 
	{
		if(tok != 1)
			printf("%d	%s\n",tok,yytext);

	}
}