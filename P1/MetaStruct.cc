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
#include <algorithm>

// stub file .. replace it with your own DBFile.cc
using namespace std;


MetaStruct::MetaStruct (const char *fPath, fType m, int n,int run=0,OrderMaker *o=NULL) {
    mode = m;
    numPages = n;
    runLength=run;
    myOrder = o;
    mPath = string(fPath) + ".meta";
}

MetaStruct::MetaStruct (const char *fPath) {
    mPath = string(fPath) + ".meta";
    myOrder =new OrderMaker();
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
    string line;
    getline(ifile,line);
    mode=static_cast<fType>(stoi(line));
    if(mode!=heap){
        ifile >> *myOrder;
        ifile >> runLength;
    }
    return 1;
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
    if(mode!=heap){
        ofile<<*myOrder;
        ofile<<runLength<<endl;
    }
    ofile.close();
    return 1;
}

void MetaStruct::incPage () {
    numPages++;
}

int MetaStruct::getPages(){
    return numPages;
}

fType MetaStruct::getType(){
    return mode;
}