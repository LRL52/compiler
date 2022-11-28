main: main.c lex.yy.c
	gcc -g -lfl main.c lex.yy.c -o main 