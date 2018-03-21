#ifndef REL_OP_H
#define REL_OP_H
#pragma ones
#include "Pipe.h"
#include "DBFile.h"
#include "Record.h"
#include "Function.h"
#include <pthread.h>
#include <iostream>
#include <cstdlib>
using namespace std;
class RelationalOp {
	
	public:
	// blocks the caller until the particular relational operator 
	// has run to completion
	void WaitUntilDone (){
		pthread_join(worker,NULL);
	}

	// tell us how much internal memory the operation can use
	virtual void Use_n_Pages (int n) = 0;
	pthread_t worker;

	//http://man7.org/linux/man-pages/man3/pthread_create.3.html
	static int create_join_thread(pthread_t *thread,void *(*start_routine)(void *),void * args);
};

class SelectFile : public RelationalOp { 

	static void* thread_work(void* arg);
	public:
	void Run (DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal);
	void Use_n_Pages (int n){};
};

class SelectPipe : public RelationalOp {
	public:
	void Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal) { }
	void WaitUntilDone () { }
	void Use_n_Pages (int n) { }
};
class Project : public RelationalOp { 
	public:
	void Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput) { }
	void WaitUntilDone () { }
	void Use_n_Pages (int n) { }
};
class Join : public RelationalOp { 
	public:
	void Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal) { }
	void WaitUntilDone () { }
	void Use_n_Pages (int n) { }
};
class DuplicateRemoval : public RelationalOp {
	public:
	void Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema) { }
	void WaitUntilDone () { }
	void Use_n_Pages (int n) { }
};
class Sum : public RelationalOp {
	public:
	void Run (Pipe &inPipe, Pipe &outPipe, Function &computeMe) { }
	void WaitUntilDone () { }
	void Use_n_Pages (int n) { }
};
class GroupBy : public RelationalOp {
	public:
	void Run (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe) { }
	void WaitUntilDone () { }
	void Use_n_Pages (int n) { }
};
class WriteOut : public RelationalOp {
	public:
	void Run (Pipe &inPipe, FILE *outFile, Schema &mySchema) { }
	void WaitUntilDone () { }
	void Use_n_Pages (int n) { }
};
#endif