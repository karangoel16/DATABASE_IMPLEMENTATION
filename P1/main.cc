
#include <iostream>
#include "ParseTree.h"
#include "Query.h"
#include "Statistics.h"
#include "test.h"
#include <string>
#include <chrono>
using namespace std;

extern "C" {
	int yyparse(void);   // defined in y.tab.c
	int yyfuncparse(void);   // defined in yyfunc.tab.c
	int yylex(void);
	int yywrap();
	void init_lexical_parser (char *src);
	void close_lexical_parser ();
	void yylex_destroy();
}


extern struct CreateTable *createTable;
extern struct InsertFile *insertFile;
extern char *dropTableName;
extern char *setOutPut;
extern	FuncOperator *finalFunction;
extern struct TableList *tables; 
extern struct AndList *boolean;
extern struct   NameList   *groupingAtts;
extern struct   NameList   *attsToSelect; 
extern int distinctAtts;
extern int distinctFunc;
extern int quit;
extern char *dropTableName;
extern char type;
std::mutex mtx;
int main () {
	setup();
	typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
	while(1){
		std::cout<<"***************************************************\n";
		std::cout<<"Type the following commands"<<endl;
		std::cout<<"CREATE TABLE"<<endl;
		std::cout<<"INSERT 'csv file'INTO 'collections' "<<endl;
		std::cout<<"SELECT to query the database"<<endl;
		std::cout<<"DROP TABLE (tablename)"<<endl;
		std::cout<<"QUIT THE DATABASE"<<endl;
		std::cout<<"***************************************************\n";
		string choice;
		//std::cin>>choice;
		//init_lexical_parser(const_cast<char *>(choice.c_str()));
		yyparse();
		auto t0 = Time::now();
		mtx.lock();
		if(quit)
		{
			mtx.unlock();
			return 1;
		}
		if(type=='c'){

			Query *q=new Query();
			if(q->CreateQuery(catalog_path,dbfile_dir,createTable)) {
				cout <<"Created table"<<createTable->tableName<<endl;
			delete q;
		}
		}else if(type=='i') {
			Query *q=new Query();
			std::cout<<insertFile->tableName<<"\n";
			if(q->InsertQuery(catalog_path,dbfile_dir,tpch_dir,insertFile))
				cout <<"Loaded file "<<insertFile->fileName<<" into " <<insertFile->tableName<<endl;
			delete q;
		}
		else if(type=='d'){
			Query *q=new Query();
			if(q->DropTable(catalog_path,dbfile_dir,dropTableName)){
				std::cout<<"We have deleted the table "<<dropTableName<<" successfully"<<endl;
			}
			else{
				std::cerr<<"Table cannot be deleted"<<endl;
			}
			delete q;
		}
		else{
			Statistics * s=new Statistics();
			s->LoadAllStatistics();
			Query *q=new Query(finalFunction,tables,boolean,groupingAtts,attsToSelect,distinctAtts,distinctFunc,s,std::string(dbfile_dir),string(tpch_dir),string(catalog_path));
			q->ExecuteQuery();
			delete q;
		}
		mtx.unlock();
		auto t1 = Time::now();
		fsec fs = t1 - t0;
    	ms d = std::chrono::duration_cast<ms>(fs);
    	std::cout << fs.count() << "s\n";
    	std::cout << d.count() << "ms\n";
		yylex_destroy();
		//close_lexical_parser ();
	}
	cleanup();
}	


