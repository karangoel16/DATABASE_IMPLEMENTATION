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
#include <mutex>

static std::unordered_map<int,Pipe *> pipe;
static int dbNum=0;
static std::unordered_map<int,DBFile *> dbs;
static std::vector<RelationalOp *> operators;
struct Node{
    Node *left=NULL,*right=NULL,*parent=NULL;
    CNF *cnf;
	Record *literal;
	Schema *outputSchema;
    RelationalOp *op;
	Function *function;
    OrderMaker *order;
    int lPipe,rPipe,oPipe,numAttsOutput,numAttsInput;
    int *keepMe;
    DBFile *db;
    Pipe *outPipe;
    string dbfilePath;
    virtual void Execute();
    virtual void Print();
    virtual void wait();
    Node():cnf(new CNF()),literal(new Record()){};
    ~Node(){
    	delete cnf;
        delete right;
	    delete literal;
    }
};

struct SelectFNode:public Node{
    SelectFile *sf=new SelectFile();
    void Execute() override;
    void Print() override;
    void wait() override;
};


struct SelectPNode:public Node{
    SelectPipe *selectPipe = new SelectPipe();
    void Execute() override;
    void Print() override;
    void wait() override;
};

struct ProjectNode:public Node{
    Project *project = new Project();
    void Execute() override;
    void Print() override;
    void wait() override;
}; 

struct JoinNode:public Node{
    Join *join = new Join;
    void Execute() override;
    void Print() override;
    void wait() override;
};

struct SumNode:public Node{
    Sum *sum = new Sum;
    void Execute() override;
    void Print() override;
    void wait() override;
};

struct GroupByNode:public Node{
    GroupBy *groupBy = new GroupBy;
    void Execute() override;
    void Print() override;
    void wait() override;
    ~GroupByNode(){
        delete groupBy;
    }
};

struct DistinctNode:public Node{
    DuplicateRemoval *dr = new DuplicateRemoval;
    void Execute() override;
    void Print() override;
    void wait() override;
    ~DistinctNode(){
        delete dr;
    }
};

struct WriteOutNode:public Node{
    WriteOut *wo = new WriteOut();
    FILE *fp;
    void Execute() override;
    void Print() override;
    void wait() override;
};

#endif