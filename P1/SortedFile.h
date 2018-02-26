#ifndef SORTEDFILE_H
#define SORTEDFILE_H

#pragma once
#include "VirtualDBFile.h"
#include "Record.h"
#include "Pipe.h"
#include "BigQ.h"
#include "Schema.h"

class BigQ;

class SortedFile: public VirtualDBFile{
    static const size_t  BUFFERSIZE=100;
    OrderMaker *myOrder;
    int runLength;
    pthread_t threadres;
    BigQ *bigQ;
    public:
    Pipe *in,*out;
    SortedFile();
    int Close () override {
        Record rec;
        in->ShutDown();
        pthread_join (threadres, NULL);
        check_write();
        file.Close();
        return 1;
    }
    static void * helper(void *args){
        SortedFile * sb=(SortedFile *)args;
        Record rec;
        while(sb->out->Remove(&rec)){
            if(!sb->writePage.Append(&rec))
            {
                int pos = sb->file.GetLength()==0? 0:sb->file.GetLength()-1; 
                sb->file.AddPage(&sb->writePage,pos);
                sb->writePage.EmptyItOut();
                sb->writePage.Append(&rec);
            }
        }
    }
    void Add (Record &addme) override;
    void setAttribute(OrderMaker *o,int run) override;
};

#endif