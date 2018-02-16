#include "BigQ.h"

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen): Pin(in),Pout(out) {
	// read data from in pipe sort them into runlen pages
    // construct priority queue over sorted runs and dump sorted data 
 	// into the out pipe
	int rc = pthread_create(&threadRes, NULL, result, (void *)this);
	if(result)
    // finally shut down the out pipe

	out.ShutDown ();
}

BigQ::~BigQ () {
}

void * BigQ::result(void *Big){
	std::cout<<"hi";
}