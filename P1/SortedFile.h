#ifndef SORTEDFILE_H
#define SORTEDFILE_H
#include "VirtualDBFile.h"
#include "Record.h"
#include "Pipe.h"
class SortedFile: public VirtualDBFile{
    static const size_t  BUFFERSIZE=128;
    Pipe *in,*out;
    public:
    SortedFile();
    void Add (Record &addme) override;
};

#endif