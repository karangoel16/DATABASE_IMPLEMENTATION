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


MetaStruct::MetaStruct (const char *fPath, fType m, int n) {
    mode = m;
    numPages = n;
    mPath = string(fPath) + ".meta";
}

MetaStruct::MetaStruct () {
}


int MetaStruct::Open () {
    std::cout<<mPath<<"\n";
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
    getline(ifile,line);
    try {
        numPages = std::stoi(line); //don't call c_str() 

        std::cout << numPages << endl;
    }
    catch(std::exception const & e)
    {
         cout<<"error : " << e.what() <<endl;
    }
    ifile.close();
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
    ofile<<numPages<<endl;
    ofile.close();
    return 1;
}

void MetaStruct::incPage () {
    numPages++;
}

int MetaStruct::getPages(){
    return numPages;
}