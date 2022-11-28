%{
#include "token.h"
#include <string.h>

#ifdef DEBUG_SCAN
char s[10005][105];
int stk[10005], top;
#endif

%}

WHITE_SPACE	[ \t\n\r]+

NEW_LINE \n|\r

COMMENT \/\*([^*]|{NEW_LINE}|(\*+([^*\/]|{NEW_LINE})))*\*+\/

SINGLE_COMMENT \/\/.*

UNICODE \\(u|U)[0-9a-fA-F]+

IDENTIFIER ({UNICODE}|[a-zA-Z_])({UNICODE}|[a-zA-Z_0-9])*

HEX (\+|\-)?0(X|x)[0-9a-fA-F]+
OCTAL (\+|\-)?0[0-7]+
DECIMAL (\+|\-)?[0-9]+

INT {HEX}|{OCTAL}|{DECIMAL}
INTEGER {INT}(u|U)?(l|L|(ll)|(LL))?

HEX_FLOAT {HEX}(\.[0-9a-fA-F]+)?((P|p){INT})?
DECIMAL_FLOAT {DECIMAL}(\.[0-9]+)?((E|e){INT})?

FLOAT {HEX_FLOAT}|{DECIMAL_FLOAT}(f|F|l|L)?

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
\/\/.*[\n]		{ /* ignore note */ }
"/*"(([^\*]*(\*[^\/])?)*)"*/"		{ /* ignore note */ }

{WHITE_SPACE}		{ }
[0-9]+		{return num_INT;}
"0"[xX][a-fA-F0-9]+		{return num_INT;}
[0-9]+\.[0-9]* { return num_FLOAT; }
[0-9]*\.[0-9]+ { return num_FLOAT; }
[0-9]+[eE][-+]*[0-9]+ { return num_FLOAT; }
[0-9]*\.[0-9]+[eE][-+]*[0-9]+ { return num_FLOAT; }
0x[0-9]*\.[0-9a-fA-F]+[pP][+-]*[0-9]* { return num_FLOAT; }
[a-zA-Z_][a-zA-Z0-9_]*		{return Y_ID;}

.	{printf("Error: 404 Not Found: %s\n", yytext); return 404;}

%%

int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}

int yywrap()
{
	return 1;
}

#ifdef DEBUG_SCAN

int main(int argc, char **argv)
{
	int tok;

	while(tok = yylex()) {
		if(tok != 1) {
			//printf("%d	%s\n",tok,yytext);
			stk[++top] = tok;
			//s[top] = yytext;
			sprintf(s[top], "%s", yytext);
		}
	}
	for(int i = 1; i <= top; ++i) {
		printf("%d %d %s\n", i, stk[i], s[i]);
	}
}

#endif