#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node_type.h"
#include "token.h"
#include "functionlist.h"

extern int yylex();
extern _YYLVAL yylval;
extern char* yytext;

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
	ret->next = ret->if_cond = NULL, ret->svalue = NULL;
	ret->ivalue = 0, ret->fvalue = 0.0;
    return ret;
}

void Free(past cur) {
	if(cur == NULL) return;
	Free(cur->if_cond);
	Free(cur->left); Free(cur->right);
	Free(cur->next);
	free(cur);
}

past PrimaryExp() {
	past ret;
	if(tok == Y_LPAR) {
		advance();
		ret = Exp();
		assert(ret != NULL);
		assert(tok == Y_RPAR);
		advance();
	} else 
	if(tok == Y_ID) {
		ret = LVal();
		assert(ret != NULL);
	} else
	if(tok == num_INT) {
		ret = NewNode(NULL, INTEGER_LITERAL, NULL);
		ret->ivalue = atoi(s[top]);
		advance();
	} else 
	if(tok == num_FLOAT) {
		ret = NewNode(NULL, FLOATING_LITERAL, NULL);
		ret->fvalue = atof(s[top]);
		advance();
	} else
		assert(0);
	return ret;
}

past UnaryExp() {
	past ret = NULL;
	if(tok == Y_ADD || tok == Y_SUB || tok == Y_NOT) {
		ret = NewNode(NULL, UNARY_OPERATOR, NULL);
		ret->svalue = s[top];
		advance();
		ret->right = UnaryExp();
	} else
	if(tok == Y_ID) {
		advance();
		if(tok == Y_LPAR) {
			ret = NewNode(NULL, CALL_EXPR, NULL); 
			ret->left = NewNode(NULL, DECL_REF_EXPR, NULL);
			ret->left->svalue = s[top - 1];
			advance();
			if(tok == Y_RPAR) { advance(); }
			else {
				ret->right = CallParams();
				assert(ret->right != NULL);
				assert(tok == Y_RPAR);
				advance();
			}
		} else {
			back();
			ret = PrimaryExp();
			assert(ret != NULL);
		}
	} else {
		ret = PrimaryExp();
		assert(ret != NULL);
	}
	return ret;
}

past MulExp() { //进入MulExp()应该是始终合法的
	past ret = UnaryExp();
	assert(ret != NULL);
	while(1) {
		if(tok != Y_MUL && tok != Y_DIV && tok != Y_MODULO) return ret;
		past t = NewNode(NULL, BINARY_OPERATOR, NULL);
		t->left = ret;
		t->svalue = s[top];
		advance();
		t->right = UnaryExp();
		ret = t;
	}
}

past AddExp() { //进入AddExp()应该是始终合法的
	//int _top = top;
	past ret = MulExp();
	assert(ret != NULL);
	while(1) {
		if(tok != Y_ADD && tok != Y_SUB) return ret;
		past t = NewNode(NULL, BINARY_OPERATOR, NULL);
		t->left = ret;
		t->svalue = s[top];
		advance();
		t->right = MulExp();
		ret = t;
	}
}

past Exp() {
	return AddExp();
}

past CallParams() {
	past ret = Exp();
	assert(ret != NULL);
	past p = ret;
	while(1) {
		if(tok != Y_COMMA) return ret;
		advance();
		past t = Exp();
		p->next = t;
		p = t;
	}
}

past ArraySubscripts() {
	assert(tok == Y_LSQUARE);
	advance();
	past ret = NewNode(NULL, ARRAY_SUBSCRIPT_EXPR, NULL);
	ret->right = Exp();
	assert(ret->right != NULL);
	assert(tok == Y_RSQUARE);
	advance();
	while(1) {
		if(tok != Y_LSQUARE) return ret;
		advance();
		past t = NewNode(ret, ARRAY_SUBSCRIPT_EXPR, NULL);
		t->right = Exp();
		assert(t->right != NULL);
		assert(tok == Y_RSQUARE);
		advance();
		ret = t;
	}
}

past LVal() {
	past ret = NewNode(NULL, DECL_REF_EXPR, NULL);
	assert(tok == Y_ID);
	ret->svalue = s[top];
	advance();
	if(tok != Y_LSQUARE) return ret;
	past p = ArraySubscripts(), t = p;
	while(t->left != NULL) t = t->left;
	t->left = ret;
	return p;
}

past RelExp() {
	past ret = AddExp(); assert(ret != NULL);
	while(1) {
		if(tok != Y_LESS && tok != Y_GREAT && tok != Y_LESSEQ && tok != Y_GREATEQ) return ret;
		past t = NewNode(ret, BINARY_OPERATOR, NULL);
		t->svalue = s[top];
		advance();
		t->right = AddExp();
		ret = t;
	}
}

past EqExp() {
	past ret = RelExp(); assert(ret != NULL);
	while(1) {
		if(tok != Y_EQ && tok != Y_NOTEQ) return ret;
		past t = NewNode(ret, BINARY_OPERATOR, NULL);
		t->svalue = s[top];
		advance();
		t->right = RelExp();
		ret = t;
	}
}

past LAndExp() {
	past ret = EqExp(); assert(ret != NULL);
	while(1) {
		if(tok != Y_AND) return ret;
		past t = NewNode(ret, BINARY_OPERATOR, NULL);
		t->svalue = s[top];
		advance();
		t->right = EqExp();
		ret = t;
	}
}

past LOrExp() {
	past ret = LAndExp(); assert(ret != NULL);
	while(1) {
		if(tok != Y_OR) return ret;
		past t = NewNode(ret, BINARY_OPERATOR, NULL);
		t->svalue = s[top];
		advance();
		t->right = LAndExp();
		ret = t;
	}
}

past Stmt() { //非空
	past ret = NULL;
	tail = ret; int _top = top;
	// if(tok == Y_ID) {
	// 	ret->nodeType = BINARY_OPERATOR;
	// 	ret->left = LVal();
	// 	if(ret->left == NULL) goto L1;
	// 	if(tok != Y_ASSIGN) goto L1;
	// 	advance();
	// 	ret->right = Exp();
	// 	if(ret->right == NULL) goto L1;
	// 	assert(tok == Y_SEMICOLON);
	// 	advance();
	// 	return ret;
	// }
	// L1: top = _top, tok = stk[top];
	if(tok == Y_SEMICOLON) {
		ret = NewNode(NULL, NULL_STMT, NULL);
		advance();
	} else
	if(tok == Y_LBRACKET) {
		ret = Block();
	} else 
	if(tok == Y_WHILE) {
		advance();
		assert(tok == Y_LPAR); advance();
		ret = NewNode(NULL, WHILE_STMT, NULL);
		ret->left = LOrExp(); assert(ret->left != NULL);
		assert(tok == Y_RPAR); advance();
		ret->right = Stmt(); assert(ret->right != NULL);
	} else
	if(tok == Y_IF) {
		advance();
		assert(tok == Y_LPAR); advance();
		ret = NewNode(NULL, IF_STMT, NULL);
		ret->if_cond = LOrExp(); assert(ret->if_cond != NULL);
		assert(tok == Y_RPAR); advance();
		ret->left = Stmt(); assert(ret->left != NULL);
		if(tok == Y_ELSE) {
			advance();
			ret->right = Stmt();
			assert(ret->right != NULL);
		}
	} else 
	if(tok == Y_BREAK) {
		advance();
		ret = NewNode(NULL, BREAK_STMT, NULL);
		assert(tok == Y_SEMICOLON); advance();
	} else
	if(tok == Y_CONTINUE) {
		advance();
		ret = NewNode(NULL, CONTINUE_STMT, NULL);
		assert(tok == Y_SEMICOLON); advance();
	} else
	if(tok == Y_RETURN) {
		ret = NewNode(NULL, RETURN_STMT, NULL);
		advance();
		if(tok == Y_SEMICOLON) advance();
		else {
			ret->left = Exp(); assert(ret->left != NULL);
			assert(tok == Y_SEMICOLON); advance();
		}
	} else {
		ret = Exp(); assert(ret != NULL); //把LVal和Exp都当Exp处理，但这样会把赋值语句左边不是左值的情况也处理成正确的了
		assert(tok == Y_ASSIGN || Y_SEMICOLON);
		enum yytokentype _tok = tok;
		top = _top, tok = stk[top]; Free(ret); //先探测，再top回溯，再进入
		if(_tok == Y_ASSIGN) {
			ret = NewNode(NULL, BINARY_OPERATOR, NULL);
			ret->left = LVal(); assert(ret->left != NULL);
			assert(tok == Y_ASSIGN); ret->svalue = s[top]; advance();
			ret->right = Exp(); assert(ret->right != NULL);
			assert(tok == Y_SEMICOLON); advance();
		} else {
			ret = Exp(); assert(ret != NULL);
			assert(tok == Y_SEMICOLON); advance();
		}
	}
	return ret;
}

past BlockItem() { //非空
	past ret = Stmt();
	return ret;
}

past BlockItems() { //非空
	past ret = NewNode(NULL, COMPOUND_STMT, NULL);
	ret->left = BlockItem();
	assert(ret->left != NULL);

	past p = ret->left, nxt;
	while(tok != Y_RBRACKET) {
		p->next = BlockItem();
		assert(p->next != NULL);
		p = p->next;
	}
	p->next = NULL;
	return ret;
}

past Block() {
    assert(tok == Y_LBRACKET);
	advance();
    past ret = BlockItems();
	assert(tok == Y_RBRACKET);
	advance();
	return ret;
}

void print(past cur) {
	enum yytokentype type = cur->nodeType;
	switch (type) {
		case COMPOUND_STMT: printf("COMPOUND_STMT"); break;
		case RETURN_STMT: printf("RETURN_STMT"); break;
		case DECL_REF_EXPR: printf("DECL_REF_EXPR %s", cur->svalue); break;
		case CALL_EXPR: printf("CALL_EXPR"); break;
		case INTEGER_LITERAL: printf("INTEGER_LITERAL %d", cur->ivalue); break;
		case FLOATING_LITERAL: printf("FLOATING_LITERAL %f", cur->fvalue); break;
		case UNARY_OPERATOR: printf("UNARY_OPERATOR"); break;
		case ARRAY_SUBSCRIPT_EXPR: printf("ARRAY_SUBSCRIPT_EXPR"); break;
		case BINARY_OPERATOR: printf("BINARY_OPERATOR %s", cur->svalue); break;
		case IF_STMT: printf("IF_STMT"); break;
		case WHILE_STMT: printf("WHILE_STMT"); break;
		case CONTINUE_STMT: printf("CONTINUE_STMT"); break;
		case BREAK_STMT: printf("BREAK_STMT"); break;
		case NULL_STMT: printf("NULL_STMT"); break;
		default: printf("Unknown node type!"); break;
	}
}

void showAst(past node, int nest) {
	if(node == NULL) return;
	int i = 0;
	for(i = 0; i < nest; i ++)
		printf("  ");
	//printf("%d\n", node->nodeType);
	print(node);
	puts("");
	showAst(node->if_cond, nest + 1);
	showAst(node->left, nest + 1);
	showAst(node->right, nest + 1);
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
