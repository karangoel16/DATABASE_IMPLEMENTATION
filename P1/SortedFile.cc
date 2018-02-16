#include "SortedFile.h"
#include <iostream>

using namespace std;

SortedFile::SortedFile(){
    NEED_TO_IMPLEMENT
    in=new Pipe(BUFFERSIZE),out=new Pipe(BUFFERSIZE);
}

void SortedFile::Add (Record &addme){
    NEED_TO_IMPLEMENT
    in->Insert(&addme);
}


