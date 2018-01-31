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
    currentPage =0;
    openFile =false;
}

//return 0 of failure and 1 and on success
int DBFile::Create (const char *f_path, fType f_type, void *startup) {
    if(openFile) 
    {
        /*
            This is done to check if the file has already been made
        */
        return 0;
    }
    openFile=true;
    metaData=MetaStruct(f_path);
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
    return 0;
}

/*
    This is done to bulk load all the records in the database
*/
void DBFile::Load (Schema &f_schema, const char *loadpath) {
    /*
        
    */
    if(!openFile)
    {
        #ifdef F_DEBUG
            std::cout<<"The file is being loaded without being either created or opened";
        #endif
        return ;
    }
    FILE * fileLoad = fopen(loadpath,"r");
    if(!fileLoad)
    {
        #ifdef verbose
            std::cout<<"File could not be opened";
            exit(1);
        #endif
    }
    Record pull;
    while(pull.SuckNextRecord(&f_schema,fileLoad))
    {
        Add(pull);
    }
}

int DBFile::Open (const char *f_path) {
    if(openFile)
    {
        #ifdef F_DEBUG 
            std::cout<<"The file is already opened and is being opened again without closing";
        #endif
        return 0;
    }
    file.Open(1,const_cast<char *>(f_path));
    metaData = MetaStruct(f_path);
    if(!metaData.Open())
    {
        std::cout<<"We had some error: E(1)";
        return 0;
    }
    //file.GetPage(&writePage,currentPage);
    openFile=true;
    return 1;
}

void DBFile::MoveFirst () {
    if(!openFile)
    {
        return;
    }
    readPage.EmptyItOut(); // to remove anything in the array
    currentPage =0;
    file.GetPage(&readPage,currentPage++);
}
//1 if it is successfully closed and 0 if it is not
int DBFile::Close () {
    if(!openFile)
    {
        #ifdef F_DEBUG
            std::cout<<"The file is already closed and we are trying to close it again";
        #endif 
        return 0;
    }    
    int pos = !file.GetLength()? 0 : file.GetLength()-2;
    file.AddPage(&writePage, pos);
    writePage.EmptyItOut();       
    metaData.Close();
    file.Close();
    return 1;
}

void DBFile::Add (Record &rec) {
    if(!writePage.Append(&rec))
    {
        int pos = file.GetLength()==0? 0:file.GetLength()-1; 
        file.AddPage(&writePage,pos);
        metaData.incPage();
        writePage.EmptyItOut();
        writePage.Append(&rec);
    }
}
//TODO this function needs to be done and yet has not been completed
int DBFile::GetNext (Record &fetchme) 
{
    while (!readPage.GetFirst(&fetchme)) {
        //std::cout<<metaData.getPages()<<"page\n";
        if(++currentPage >= metaData.getPages()) 
        {
            std::cout<<currentPage<<" "<<metaData.getPages()<<"\n";
            return 0;  // no more records
        }
        file.GetPage(&readPage, currentPage);
    }
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
