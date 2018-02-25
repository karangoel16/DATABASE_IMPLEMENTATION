CC = g++ -std=c++11 -O2 -Wno-deprecated 

tag = -i

ifdef linux
tag = -n
endif

a1-test.out: Record.o BigQ.o SortedFile.o Comparison.o VirtualDBFile.o BigQ.o Pipe.o ComparisonEngine.o HeapFile.o Schema.o File.o DBFile.o y.tab.o lex.yy.o MetaStruct.o BigQ.o SortedFile.o a1-test.o
	$(CC) -D F_DEBUG -o a1-test.out BigQ.o Pipe.o VirtualDBFile.o HeapFile.o SortedFile.o Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o y.tab.o lex.yy.o MetaStruct.o a1-test.o -lfl -lpthread
	
a2-test.out: Record.o BigQ.o SortedFile.o Comparison.o VirtualDBFile.o Pipe.o ComparisonEngine.o HeapFile.o Schema.o File.o DBFile.o y.tab.o lex.yy.o MetaStruct.o a2-test.o
	$(CC) -D F_DEBUG -o a2-test.out BigQ.o Pipe.o VirtualDBFile.o HeapFile.o SortedFile.o Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o y.tab.o lex.yy.o MetaStruct.o a2-test.o -lfl -lpthread

test.out: Record.o BigQ.o SortedFile.o Comparison.o VirtualDBFile.o Pipe.o ComparisonEngine.o HeapFile.o Schema.o File.o DBFile.o y.tab.o lex.yy.o MetaStruct.o test.o
	$(CC) -D F_DEBUG -o test.out BigQ.o Pipe.o VirtualDBFile.o HeapFile.o SortedFile.o Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o y.tab.o lex.yy.o MetaStruct.o test.o -lfl -lpthread

main: Record.o Comparison.o ComparisonEngine.o Schema.o File.o y.tab.o lex.yy.o main.o
	$(CC) -o main Record.o Comparison.o ComparisonEngine.o Schema.o File.o y.tab.o lex.yy.o main.o -lfl

MetaStruct.o: MetaStruct.cc
	$(CC) -g -c MetaStruct.cc

test.o: test.cc
	$(CC) -g -c test.cc

a1-test.o: a1-test.cc
	$(CC) -g -c a1-test.cc

a2-test.o: a2-test.cc
	$(CC) -g -c a2-test.cc


main.o: main.cc
	$(CC) -g -c main.cc
	
Comparison.o: Comparison.cc
	$(CC) -g -c Comparison.cc
	
ComparisonEngine.o: ComparisonEngine.cc
	$(CC) -g -c ComparisonEngine.cc
	
DBFile.o: DBFile.cc
	$(CC) -g -c DBFile.cc

File.o: File.cc
	$(CC) -g -c File.cc

Record.o: Record.cc
	$(CC) -g -c Record.cc

BigQ.o: Pipe.cc
	$(CC) -g -c BigQ.cc

Pipe.o: Pipe.cc
	$(CC) -g -c Pipe.cc

Schema.o: Schema.cc
	$(CC) -g -c Schema.cc

HeapFile.o: HeapFile.cc
	$(CC) -g -c HeapFile.cc

SortedFile.o: SortedFile.cc
	$(CC) -g -c SortedFile.cc

VirtualDBFile.o: VirtualDBFile.cc
	$(CC) -g -c VirtualDBFile.cc
	
y.tab.o: Parser.y
	yacc -d Parser.y
	sed $(tag) y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c y.tab.c

lex.yy.o: Lexer.l
	lex  Lexer.l
	gcc  -c lex.yy.c

clean: 
	rm -f *.o
	rm -f *.out
	rm -f y.tab.c
	rm -f lex.yy.c
	rm -f y.tab.h

all: a2-test.out a1-test.out test.out