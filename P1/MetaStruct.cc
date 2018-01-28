#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "MetaStruct.h"
#include "Defs.h"
#include <iostream>

// stub file .. replace it with your own DBFile.cc
using namespace std;


MetaStruct::MetaStruct () {
    mode = heap;
    numRecords =0;
}

int MetaStruct::Open (const char *fpath) {

} 

int MetaStruct::Close () {

}