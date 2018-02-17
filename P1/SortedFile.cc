#include "SortedFile.h"
#include "BigQ.h"
#include <iostream>

using namespace std;

SortedFile::SortedFile(){
    myOrder=new OrderMaker();
    in=new Pipe(BUFFERSIZE),out=new Pipe(BUFFERSIZE);
}

void SortedFile::Add (Record &addme){
    in->Insert(&addme);
}

void SortedFile::setAttribute(OrderMaker *o,int run){
    myOrder=o;
    runLength=run;
    bigQ=new BigQ(*in,*out,*myOrder,runLength);
}



