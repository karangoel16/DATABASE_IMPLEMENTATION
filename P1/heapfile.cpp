#include "File.h"
#include <iostream>
using namespace std;
class HeapFile :public File{
    public:
    HeapFile();
    ~HeapFile();
    void AddPage (Page *addMe, off_t whichPage);
};

HeapFile::HeapFile(){
    File();
}
void HeapFile :: AddPage (Page *addMe, off_t whichPage)
{
    std::cout<<"Hello";
    return ;
}