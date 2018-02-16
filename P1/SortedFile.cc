#include "SortedFile.h"
#include "BigQ.h"
#include <iostream>

using namespace std;

SortedFile::SortedFile(){
    in=new Pipe(BUFFERSIZE),out=new Pipe(BUFFERSIZE);
}

void SortedFile::Add (Record &addme){
    in->Insert(&addme);
}


