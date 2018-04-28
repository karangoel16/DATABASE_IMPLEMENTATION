#ifndef QUERY_H
#define QUERY_H_
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
#include "TreeNode.h"
#include <map>
#include <cstring>
#include <mutex>
struct Node;


class Query{
    std::mutex mtx;
    std::vector<RelationalOp *> operators;
    Node *root;//this is the main Node to be used later 
    struct FuncOperator * finalFunction; //function in aggregation
	struct TableList * tables;   // Tables in FROM CLAUSE
	struct AndList *cnfAndList;  // AndList from WHER CLAUSE
	struct NameList * groupAtts; // grouping atts (NULL if no grouping)
	struct NameList * selectAtts; // the set of attributes in the SELECT (NULL if no such atts)
	int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
	int distinctFunc;  // 1 if there is a DISTINCT in an aggregate query
    Statistics *s;
    int pipeSelect=0;
    std::unordered_map<string, AndList *> Selectors(std::vector<AndList *> list);
    void JoinsAndSelects(std::vector<AndList*> &joins, std::vector<AndList*> &selects,std::vector<AndList*> &selAboveJoin); 
    Function *GenerateFunc(Schema *schema);
    OrderMaker *GenerateOM(Schema *schema);
    std::unordered_map<string, AndList*>OptimizeSelectAndApply(vector<AndList*> selects);
    vector<AndList*> OptimizeJoinOrder(vector<AndList*> joins);
    public:
    Query(){};
    Query(struct FuncOperator *finalFunction,
			struct TableList *tables,
			struct AndList * boolean,
			struct NameList * pGrpAtts,
	        struct NameList * pAttsToSelect,
	        int distinct_atts, int distinct_func, Statistics *s,string dir,string tpch,string catalog);
    void ExecuteQuery();
    void PrintQuery();
    bool DropTable(string catalog_path,string dir,string name);
    bool CreateQuery(string catalog_path,string dir,CreateTable *create);
    bool InsertQuery(string catalog_path,string dir,string tpch_dir,InsertFile *insert);
    ~Query(){
        delete finalFunction;
        delete tables;
        delete root;
        delete cnfAndList;
        delete groupAtts;
        delete selectAtts;
        delete s;
	    }
};

#endif