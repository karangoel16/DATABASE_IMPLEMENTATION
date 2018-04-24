
#include <iostream>
#include "ParseTree.h"
#include "Query.h"
#include "Statistics.h"
#include "test.h"
#include <string>
using namespace std;

extern "C" {
	int yyparse(void);   // defined in y.tab.c
	int yyfuncparse(void);   // defined in yyfunc.tab.c
}

extern	FuncOperator *finalFunction;
extern struct TableList *tables; 
extern struct AndList *boolean;
extern struct   NameList   *groupingAtts;
extern struct   NameList   *attsToSelect; 
extern int distinctAtts;
extern int distinctFunc;
extern int quit;
extern char *dropTableName;
int main () {
	setup();
	while(1){
		std::cout<<"***************************************************\n";
		std::cout<<"Type the following commands"<<endl;
		std::cout<<"CREATE TABLE"<<endl;
		std::cout<<"SELECT to query the database"<<endl;
		std::cout<<"DROP TABLE (tablename)"<<endl;
		std::cout<<"QUIT THE DATABASE"<<endl;
		std::cout<<"***************************************************\n";
		yyparse();
		if(quit){
			break;
		}
		else if(dropTableName){
			Query *q=new Query();
			if(q->DropTable(catalog_path,dbfile_dir,dropTableName)){
				std::cout<<"We have deleted the table "<<dropTableName<<" successfully"<<endl;
			}
			else{
				std::cerr<<"Table cannot be deleted"<<endl;
			}
		}
		else{
			Statistics * s=new Statistics();
			s->LoadAllStatistics();
			Query *q=new Query(finalFunction,tables,boolean,groupingAtts,attsToSelect,distinctAtts,distinctFunc,s,std::string(dbfile_dir),string(tpch_dir),string(catalog_path));
			q->PrintQuery();
			q->ExecuteQuery();
		}
	}
	cleanup();
}	


