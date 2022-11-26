#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node_type.h"
#include "token.h"

extern int yylex();
extern _YYLVAL yylval;
extern char* yytext;

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

#define MAXN 1005
enum yytokentype tok;
char s[MAXN][MAXN];
int stk[MAXN], top;
past tail;

void advance() {
	static int flag = 0, MAXTOP = 0;
	if(!flag && top == MAXTOP) {
		tok = yylex();
		stk[++top] = tok;
		sprintf(s[top], "%s", yytext);
		if(top > MAXTOP) MAXTOP = top;
		if(tok == 0) flag = 1;
	} else {
		tok = stk[++top];
	}
	//printf("tok: %s\n", yytext);
}

void back() {
	tok = stk[--top];
}

past NewNode(past l, node_type type, past r) {
    past ret = (past)malloc(sizeof(ast));
    if(ret == NULL) {
        printf("run out of memory.\n");
        exit(0);
    }
    ret->left = l;
    ret->right = r;
    ret->nodeType = type;
    return ret;
}

past Stmt() { //非空
	past ret = NewNode(NULL, NULL_STMT, NULL);
	tail = ret;
	if(tok == Y_RETURN) {
		ret->nodeType = RETURN_STMT;
		advance();
		if(tok == Y_SEMICOLON) return ret;
	} else {
		back();
		free(ret);
		return NULL;
	}
}

past BlockItem() { //非空
	past ret = Stmt();
	return ret;
}

past BlockItems() { //非空
	past ret = NewNode(NULL, COMPOUND_STMT, NULL);
	ret->left = BlockItem();
	
	past p = ret->left, nxt;
	advance();
	while((nxt = BlockItem()) != NULL) {
		p->next = nxt;
		p = tail;
		advance();
	}
	p->next = NULL;
	return ret;
}

past Block() {
    assert(tok == Y_LBRACKET);
	advance();
    past ret = BlockItems();
	advance();
	assert(tok == Y_RBRACKET);
	return ret;
}

void print(enum yytokentype type) {
	switch (type) {
		case COMPOUND_STMT: printf("%s", "COMPOUND_STMT"); break;
		case RETURN_STMT: printf("%s", "RETURN_STMT"); break;
		default: printf("%s", "Unknown node type!"); break;
	}
}

void showAst(past node, int nest)
{
	if(node == NULL)
		return;

	int i = 0;
	for(i = 0; i < nest; i ++)
		printf("  ");
	//printf("%d\n", node->nodeType);
	print(node->nodeType);
	puts("");
	showAst(node->left, nest+1);
	showAst(node->right, nest+1);
	showAst(node->next, nest);
}

int main(int argc, char **argv)
{
	freopen("test.in", "r", stdin);
	//printf("input expression: \n");
	advance();
	// int r = expr();
	// printf("result: %d\n", r);

	past rr = Block();
	showAst(rr, 0);
	return 0;
}
