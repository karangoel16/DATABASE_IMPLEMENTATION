CC = g++ -std=c++11 -O2 -w

tag = -i

ifdef linux
tag = -n
endif

a3.out: Record.o Comparison.o Statistics.o ComparisonEngine.o Schema.o File.o DBFile.o Pipe.o BigQ.o RelOp.o Function.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o VirtualDBFile.o SortedFile.o MetaStruct.o HeapFile.o a3.o
	$(CC) -o a3.out Record.o Statistics.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o VirtualDBFile.o SortedFile.o MetaStruct.o HeapFile.o Pipe.o BigQ.o Function.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o RelOp.o a3.o -lfl -lpthread

test.out: Record.o Comparison.o Statistics.o ComparisonEngine.o Schema.o File.o DBFile.o Pipe.o BigQ.o RelOp.o Function.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o VirtualDBFile.o SortedFile.o MetaStruct.o HeapFile.o test.o
	$(CC) -o test.out Record.o Statistics.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o VirtualDBFile.o SortedFile.o MetaStruct.o HeapFile.o Pipe.o BigQ.o Function.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o RelOp.o test.o -lfl -lpthread

a1-test.out: Record.o BigQ.o Statistics.o SortedFile.o Comparison.o VirtualDBFile.o BigQ.o Pipe.o ComparisonEngine.o Function.o HeapFile.o Schema.o File.o DBFile.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o MetaStruct.o BigQ.o SortedFile.o a1-test.o
	$(CC) -D F_DEBUG -o a1-test.out Statistics.o BigQ.o Pipe.o VirtualDBFile.o HeapFile.o SortedFile.o Record.o Function.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o MetaStruct.o a1-test.o -lfl -lpthread
	
a2-test.out: Record.o BigQ.o Statistics.o SortedFile.o Comparison.o VirtualDBFile.o Pipe.o ComparisonEngine.o Function.o HeapFile.o Schema.o File.o DBFile.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o MetaStruct.o a2-test.o
	$(CC) -D F_DEBUG -o a2-test.out Statistics.o BigQ.o Pipe.o VirtualDBFile.o HeapFile.o SortedFile.o Record.o Comparison.o ComparisonEngine.o  Function.o Schema.o File.o DBFile.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o MetaStruct.o a2-test.o -lfl -lpthread

a22.out: Record.o BigQ.o SortedFile.o Statistics.o Comparison.o VirtualDBFile.o Pipe.o ComparisonEngine.o Function.o HeapFile.o Schema.o File.o DBFile.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o MetaStruct.o a22.o
	$(CC) -D F_DEBUG -o a22.out BigQ.o Pipe.o Statistics.o VirtualDBFile.o HeapFile.o SortedFile.o Record.o Comparison.o ComparisonEngine.o Function.o  Schema.o File.o DBFile.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o MetaStruct.o a22.o -lfl -lpthread

main: Record.o Comparison.o ComparisonEngine.o Schema.o File.o y.tab.o lex.yy.o main.o
	$(CC) -o main Record.o Comparison.o ComparisonEngine.o Schema.o File.o y.tab.o lex.yy.o main.o -lfl

MetaStruct.o: MetaStruct.cc
	$(CC) -g -c MetaStruct.cc

a22.o: a22.cc
	$(CC) -g -c a22.cc

a1-test.o: a1-test.cc
	$(CC) -g -c a1-test.cc

a2-test.o: a2-test.cc
	$(CC) -g -c a2-test.cc

a3.o: a3.cc
	$(CC) -g -c a3.cc

main.o: main.cc
	$(CC) -g -c main.cc
	
Comparison.o: Comparison.cc
	$(CC) -g -c Comparison.cc

Statistics.o: Statistics.cc
	$(CC) -g -c Statistics.cc
	
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

test.o: test.cc
	$(CC) -g -c test.cc

HeapFile.o: HeapFile.cc
	$(CC) -g -c HeapFile.cc

SortedFile.o: SortedFile.cc
	$(CC) -g -c SortedFile.cc

VirtualDBFile.o: VirtualDBFile.cc
	$(CC) -g -c VirtualDBFile.cc

RelOp.o: RelOp.cc
	$(CC) -g -c RelOp.cc

Function.o: Function.cc
	$(CC) -g -c Function.cc
	
y.tab.o: Parser.y
	yacc -d Parser.y
	#sed $(tag) y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c y.tab.c
		
yyfunc.tab.o: ParserFunc.y
	yacc -p "yyfunc" -b "yyfunc" -d ParserFunc.y
	#sed $(tag) yyfunc.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c yyfunc.tab.c
	
lex.yy.o: Lexer.l
	lex Lexer.l
	gcc  -c lex.yy.c

lex.yyfunc.o: LexerFunc.l
	lex -Pyyfunc LexerFunc.l
	gcc  -c lex.yyfunc.c

clean: 
	rm -f *.o
	rm -f *.out
	rm -f y.tab.*
	rm -f lex.yy.*
	rm -f y.tab.*
	rm -f yyfunc.tab.*
	rm -f lex.yyfunc*

all: a2-test.out a1-test.out a22.out a3.out test.out