#include "VirtualDBFile.h"

void VirtualDBFile::MoveFirst(){
    check_write();
    readPage.EmptyItOut(); // to remove anything in the array
    file.GetPage(&readPage,currentPage=0);
}

int VirtualDBFile::Close(){
    check_write();
    file.Close();
    return 1;
}

int VirtualDBFile::GetNext(Record &fetchme){
    check_write(); //to check if we need to write before getNext in case some records have been written in the file
    while (!readPage.GetFirst(&fetchme)) {
        if(++currentPage >=file.GetLength()-1) 
            return 0;
        file.GetPage(&readPage, currentPage);
    }
    return 1;
}

int VirtualDBFile::GetNext(Record &fetchme, CNF &cnf, Record &literal){
    ComparisonEngine comp;
    while(GetNext(fetchme))
    {
        if(comp.Compare(&fetchme, &literal, &cnf)) 
        {
            return 1;
        }
    }
    return 0;
}

void VirtualDBFile::Load (Schema &myschema, const char *loadpath){
    FILE * fileLoad = fopen(loadpath,"r");
    if(!fileLoad)
    {
        #ifdef F_DEBUG
            std::cout<<"File could not be opened";
            exit(1);
        #endif
    }
    Record pull;
    while(pull.SuckNextRecord(&myschema,fileLoad))
    {
        Add(pull);
    }
}

File VirtualDBFile::getFile(){
    return file;
}
