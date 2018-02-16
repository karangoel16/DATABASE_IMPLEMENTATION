#ifndef HEAPFILE_H
#define HEAPFILE_H
#include "VirtualDBFile.h"
#include "Record.h"
#include "DBFile.h"
class HeapFile:public VirtualDBFile{
    public:
    HeapFile();
    void Add (Record &addme) override; 
};

#endif