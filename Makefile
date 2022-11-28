all: main test scan
main: main.c lex.yy.c
	gcc -g -lfl main.c lex.yy.c -o main
lex.yy.c: scan.l
	flex scan.l
test: test.cpp
	g++ test.cpp -o test
scan: lex.yy.c
	gcc -lfl -DDEBUG_SCAN lex.yy.c -o scan