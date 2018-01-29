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
#include <fstream>

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
    ifstream ifile(mPath);
    if(!ifile)
    {
        #ifdef F_DEBUG
            cout<<"File could not be opened"; 
        #endif
        return 0;
    }

} 

int MetaStruct::Close () {
    ofstream ofile(mPath);
    if(!ofile)
    {
        #ifdef F_DEBUG
            cout<<"File could not be opened";
        #endif
        return 0;
    }
    ofile<<mode<<endl;
    ofile<<numPages<<endl;
    ofile.close();
    return 1;
}

void MetaStruct::incPage () {
    numPages++;
}