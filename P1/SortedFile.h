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
    static const size_t  BUFFERSIZE=128;
    Pipe *in,*out;
    OrderMaker *myOrder;
    int runLength;
    BigQ *bigQ;
    public:
    SortedFile();
    void Add (Record &addme) override;
    void setAttribute(OrderMaker *o,int run) override;
};

#endif