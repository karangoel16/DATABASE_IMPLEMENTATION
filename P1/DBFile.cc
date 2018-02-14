#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>

// stub file .. replace it with your own DBFile.cc
using namespace std;


DBFile::DBFile () {
    //dbfile->currentPage =0;
    openFile =false;

}

//return 0 of failure and 1 and on success
int DBFile::Create (const char *f_path, fType f_type, void *startup) {
    if(openFile) 
    {
        /*
            This is done to check if the file has already been made
        */
        #ifdef F_DEBUG
            std::cout<<"File already is opened when we are trying to create";
        #endif
        return 0;
    }
    openFile=true;
    metaData=MetaStruct(f_path);
    switch(f_type)
    {
        case heap:
                dbfile=new HeapFile();
                dbfile->file.Open(0,const_cast<char*>(f_path));
                return 1;
        case sorted:
                dbfile = new SortedFile();
                break;
        case tree: 
        default: std::cout<<"The mode has not been implemented"<<endl;
    }
    return 0;
}

/*
    This is done to bulk load all the records in the database
*/
void DBFile::Load (Schema &f_schema, const char *loadpath) {
    /*
        This is done to check if the file exist 
    */
    if(!openFile)
    {
        #ifdef F_DEBUG
            std::cout<<"The file is being loaded without being either created or opened";
        #endif
        return ;
    }
    dbfile->Load(f_schema,loadpath);
    return ;
}

int DBFile::Open (const char *f_path) {
     if(openFile)
    {
        #ifdef F_DEBUG
            std::cout<<"The file is already opened and is being opened again without closing";
        #endif
        return 0;
    }
    metaData = MetaStruct(f_path);
    if(!metaData.Open())
    {
        std::cout<<"We had some error: E(1)\n";
        return 0;
    }
    openFile=true;
    switch(metaData.getType())
    {
        case heap:dbfile = new HeapFile();
            break;
        case sorted:dbfile = new SortedFile();
            break;
        case tree:
        default:NEED_TO_IMPLEMENT
        return 0;
    }
    dbfile->file.Open(1,const_cast<char *>(f_path));
    return 1;
}

void DBFile::MoveFirst () {
    if(!openFile || !dbfile)
    {
        #ifdef F_DEBUG
            std::cout<<"The file is not opened and read record is being moved to first";
        #endif
        return;
    }
    //this is done to write the record which are still in the cache and needs to be written in file before any things starts
    dbfile->MoveFirst();
}
//1 if it is successfully closed and 0 if it is not
int DBFile::Close () { 
    //TODO to check if a particular file has some records to be written or not
    if(!openFile || !dbfile)
    {
        #ifdef F_DEBUG
            std::cout<<"The file has we are trying to close has already been closed\n";
        #endif
        return 0;
    }
    openFile = false; //we have closed the file again
    dbfile->Close();
    metaData.Close();
    return 1;
}

void DBFile::Add (Record &rec) {
    if(!openFile || !dbfile)
    {
        #ifdef F_DEBUG
            std::cout<<"The file is already closed and we are trying to close it again";
        #endif 
        return ;
    }  
    dbfile->Add(rec);
}
//TODO this function needs to be done and yet has not been completed
int DBFile::GetNext (Record &fetchme) 
{
    if(!openFile || !dbfile){
        #ifdef F_DEBUG
            std::cout<<"The file is already closed and we are trying to read it again";
        #endif
        return 0;
    }
    return dbfile->GetNext(fetchme);
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    if(!openFile || !dbfile){
        #ifdef F_DEBUG
            std::cout<<"The file is already closed and we are trying to read it again";
        #endif
        return 0;
    }
    return dbfile->GetNext(fetchme,cnf,literal);
}
