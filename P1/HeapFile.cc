#include "HeapFile.h"
#include <iostream>
using namespace std;

HeapFile::HeapFile(){
}

void HeapFile::Add (Record &rec){
    //NEED_TO_IMPLEMENT  
    if(!writePage.Append(&rec))
    {
        int pos = file.GetLength()==0? 0:file.GetLength()-1; 
        file.AddPage(&writePage,pos);
        writePage.EmptyItOut();
        writePage.Append(&rec);
    }
    return ;
}

