#ifndef METASTRUCT_H
#define METASTRUCT_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include<string>

typedef enum {heap, sorted, tree} fType;

class MetaStruct {

private:
	fType mode;
	int numPages;
    string mPath;
public:
    MetaStruct ();
	MetaStruct (const char *fpath, fType m=heap, int n=0); 


	int Open ();
	int Close ();
    void incPage();
    void setMode();
	int getPages();
};
#endif