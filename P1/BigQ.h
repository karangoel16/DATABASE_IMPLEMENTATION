#ifndef BIGQ_H
#define BIGQ_H
#include <pthread.h>
#include <iostream>
#include "Pipe.h"
#include "File.h"
#include "Record.h"

using namespace std;

class BigQ {
	Pipe &Pin, &Pout;
	pthread_t threadRes;
public:

	BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
	~BigQ ();
	//this function will remain same for everyone
	static void * result(void *bigQ);
};

#endif
