#ifndef OPTIMIZER_H_
#define OPTIMIZER_H_
#include "Function.h"
#include "Statistics.h"
#include "ComparisonEngine.h"
#include "Record.h"
#include <unordered_map>
#include "Pipe.h" 
#include "RelOp.h"
#include "Query.h"
#include <iostream>
#include "DBFile.h"

enum QueryType{
    SELECTF,SELECTP,SUM,DISTINCT,JOIN,PROJECT,GROUPBY,WRITEOUT
};

struct Node{
    Node *left,*right,*parent;
    QueryType opType;
    CNF *cnf;
	Record *literal;
	Schema *outputSchema;
	Function *function;
    OrderMaker *order;
    int lPipe,rPipe,oPipe,numAttsOutput;
    int *keepMe;
    DBFile *db;
    Pipe *outPipe;
    string dbfilePath;
    void Execute();
    void Print();
};

class Query{
    Node *root;//this is the main Node to be used later 
    struct FuncOperator * finalFunction; //function in aggregation
	struct TableList * tables;   // Tables in FROM CLAUSE
	struct AndList *cnfAndList;  // AndList from WHER CLAUSE
	struct NameList * groupAtts; // grouping atts (NULL if no grouping)
	struct NameList * selectAtts; // the set of attributes in the SELECT (NULL if no such atts)
	int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
	int distinctFunc;  // 1 if there is a DISTINCT in an aggregate query
    Statistics *s;
    unordered_map<int,Pipe *> pipe;
    public:
    Query(struct FuncOperator *finalFunction,
			struct TableList *tables,
			struct AndList * boolean,
			struct NameList * pGrpAtts,
	        struct NameList * pAttsToSelect,
	        int distinct_atts, int distinct_func, Statistics *s):
            root(nullptr),
            finalFunction(finalFunction),
            tables(tables),
            cnfAndList(boolean),
            groupAtts(pGrpAtts),
            selectAtts(pAttsToSelect),
            distinctAtts(distinctAtts),
            distinctFunc(distinct_func),
            s(s)
            {}
    void ExecuteQuery();
    void PrintQuery();
};
#endif