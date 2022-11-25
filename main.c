#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node_type.h"
#include "token.h"

extern int yylex();
extern int yylval;
extern char* yytext;

int tok;

void advance()
{
	tok = yylex();
	printf("tok: %s\n", yytext);
}

typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
	int ivalue;
	float fvalue;
	char* svalue;
	node_type nodeType;
	past left;
	past right;
	past if_cond;
	past next;
};

past NewNode(past l, node_type type, past r) {
    past ret = malloc(sizeof(ast));
    if(ret == NULL) {
        printf("run out of memory.\n");
        exit(0);
    }
    ret->left = l;
    ret->right = r;
    ret->nodeType = type;
    return ret;
}

past Block() {
    assert(tok == Y_LBRACKET);
    
}

// void showAst(past node, int nest)
// {
// 	if(node == NULL)
// 		return;

// 	int i = 0;
// 	for(i = 0; i < nest; i ++)
// 		printf("  ");
// 	if(strcmp(node->nodeType, "intValue") == 0)
// 		printf("%s %d\n", node->nodeType, node->ivalue);
// 	else if(strcmp(node->nodeType, "expr") == 0)
// 		printf("%s '%c'\n", node->nodeType, (char)node->ivalue);
// 	showAst(node->left, nest+1);
// 	showAst(node->right, nest+1);

// }

int main(int argc, char **argv)
{
	while(1)
	{
		printf("input expression, 'q' to exit>");
		advance();
		// int r = expr();
		// printf("result: %d\n", r);

		past rr = Block();
		showAst(rr, 0);
	}
	return 0;
}
