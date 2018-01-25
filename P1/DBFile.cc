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

// stub file .. replace it with your own DBFile.cc
using namespace std;


DBFile::DBFile () {
}

//return 0 of failure and 1 and on success
int DBFile::Create (const char *f_path, fType f_type, void *startup) {
    switch(f_type)
    {
        case heap:
                file.Open(0,const_cast<char*>(f_path));
                //std::cout<<file.GetPage()<<"\n";
                return 1;
        case sorted:
        case tree: 
        default: std::cout<<"The mode has not been implemented"<<endl;
    }
    return 0;
}

void DBFile::Load (Schema &f_schema, const char *loadpath) {

}

int DBFile::Open (const char *f_path) {
    file.Open(1,const_cast<char *>(f_path));
    //file.GetPage(&page,file.GetLength());
    return 1;
}

void DBFile::MoveFirst () {
}
//1 if it is successfully closed and 0 if it is not
int DBFile::Close () {
    if(file.Close())
        return 1;
    return 0;
}

void DBFile::Add (Record &rec) {
    if(!page.Append(&rec))
    {
        file.AddPage(&page,file.GetLength());
        page.Append(&rec);
    }
}

int DBFile::GetNext (Record &fetchme) {
    
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {

}
