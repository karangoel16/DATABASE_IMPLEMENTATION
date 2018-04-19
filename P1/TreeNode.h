#ifndef OPTIMIZER_H_
#define OPTIMIZER_H_
#include "Function.h"
#include "Statistics.h"
#include "ComparisonEngine.h"
#include "Record.h"
#include <unordered_map>
#include "Pipe.h" 
#include "RelOp.h"
#include <iostream>
#include "DBFile.h"
#include <vector>
#include "ParseTree.h"

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
    virtual void Execute();
    virtual void Print();
    Node(){};
    Node(QueryType type,Node *parent,int lPipe,int rPipe,int oPipe,string dbfile):
    opType(type),
    left(nullptr),
    right(nullptr),
    parent(parent),
    dbfilePath(dbfile)
    {};
};

struct SelectFNode:public Node{
    void Execute() override;
    void Print() override;
};


struct SelectPNode:public Node{
    void Execute() override;
    void Print() override;
};

struct ProjectNode:public Node{
    void Execute() override;
    void Print() override;
}; 

struct JoinNode:public Node{
    void Execute() override;
    void Print() override;
};

struct SumNode:public Node{
    void Execute() override;
    void Print() override;
};

struct GroupByNode:public Node{
    void Execute() override;
    void Print() override;
};

struct DistinctNode:public Node{
    void Execute() override;
    void Print() override;
};

struct WriteOutNode:public Node{
    void Execute() override;
    void Print() override;
};

#endif