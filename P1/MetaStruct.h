#ifndef METASTRUCT_H
#define METASTRUCT_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

typedef enum {heap, sorted, tree} fType;

class MetaStruct {
private:
	fType mode;
	int numRecords;
public:
	MetaStruct (); 

	int Open (const char *fpath);
	int Close ();
};
#endif