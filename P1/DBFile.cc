#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>
#include <queue>
#include "heapfile.cpp"

// stub file .. replace it with your own DBFile.cc
using namespace std;


DBFile::DBFile () {
    currentPage =0;
    openFile =false;
}

//return 0 of failure and 1 and on success
int DBFile::Create (const char *f_path, fType f_type, void *startup) {
    openFile=true;
    switch(f_type)
    {
        case heap:
                file.Open(0,const_cast<char*>(f_path));
                std::cout<<file.GetLength()<<"\n";
                return 1;
        case sorted:
        case tree: 
        default: std::cout<<"The mode has not been implemented"<<endl;
    }
    //file.GetPage(&page,0);
    return 0;
}

void DBFile::Load (Schema &f_schema, const char *loadpath) {
    FILE * fileLoad = fopen(loadpath,"r");
    if(!fileLoad)
    {
        #ifdef verbose
            std::cout<<"File "+ loadpath+" could not be opened";
            exit(1);
        #endif
    }
    Record pull;
    while(pull.SuckNextRecord(&f_schema,fileLoad))
    {
        pull.Print(&f_schema);
    }
}

int DBFile::Open (const char *f_path) {
    file.Open(1,const_cast<char *>(f_path));
    file.GetPage(&writePage,currentPage);
    openFile=true;
    return 1;
}

void DBFile::MoveFirst () {
    
}
//1 if it is successfully closed and 0 if it is not
int DBFile::Close () {
    if(!openFile)
    {
        return 0;
    }    
    int pos = !file.GetLength()? 0 : file.GetLength()-2;
    file.AddPage(&writePage, pos);
    writePage.EmptyItOut();       
    file.Close();
    return 1;
}

void DBFile::Add (Record &rec) {
    if(!writePage.Append(&rec))
    {
        int pos = file.GetLength()==0? 0:file.GetLength()-1; 
        file.AddPage(&writePage,pos);
        writePage.EmptyItOut();
        writePage.Append(&rec);
    }
}

int DBFile::GetNext (Record &fetchme) {
    return 1;
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
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
