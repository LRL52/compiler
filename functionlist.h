#ifndef __FUNCTIONLIST_H__
#define __FUNCTIONLIST_H__
#include "node_type.h"
#include "token.h"

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

void advance();
void back();
past NewNode(past l, node_type type, past r);
void Free(past cur);
past PrimaryExp();
past UnaryExp();
past MulExp();
past AddExp();
past Exp();
past CallParams();
past ArraySubscripts();
past LVal();
past RelExp();
past EqExp();
past LAndExp();
past LOrExp();
past Stmt();
past BlockItem();
past BlockItems();
past Block();
void print(past cur);
void showAst(past node, int nest);

#endif