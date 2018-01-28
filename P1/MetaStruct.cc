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


MetaStruct::MetaStruct (const char *fPath, fType m, int n) {
    mode = m;
    numPages = n;
    mPath = string(fPath) + ".meta";
    cout << mPath;
}

MetaStruct::MetaStruct () {

}


int MetaStruct::Open () {

} 

int MetaStruct::Close () {

}

void MetaStruct::incPage () {

}