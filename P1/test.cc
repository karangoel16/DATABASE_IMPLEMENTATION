#include <iostream>
#include "DBFile.h"
#include "gtest/gtest.h"
#include "test.h"
#include "File.h"
#include "Schema.h"
#include <vector>
#include <string>

// make sure that the file path/dir information below is correct
const char *dbfile_dir = ""; // dir where binary heap files should be stored
string tpch_dir ="/home/karan/Downloads/GIT_HUB/tpch-dbgen/"; // dir where dbgen tpch files (extension *.tbl) can be found
const char *catalog_path = "catalog"; // full path of the catalog file

using namespace std;

relation *rel;
vector<relation *> rel_ptr;
TEST (DB_FILE,create) {
    for (auto i:rel_ptr){
        DBFile dbfile;
	    dbfile.Create (i->path(), heap, NULL);
        string tbl_path=tpch_dir+i->name()+".tbl";
        FILE *tableFile = fopen (&tbl_path[0u], "r");
        //cout << " tpch file will be loaded from " << tbl_path << endl;
        dbfile.Load (*(i->schema ()), &tbl_path[0u]);
        Record temp;
        Schema mySchema ("catalog", i->name());
        while(temp.SuckNextRecord(&mySchema,tableFile)){
            #ifdef verbose
                temp.Print(&mySchema);
            #endif
            dbfile.Add(temp);
        }
        dbfile.Close ();
    }
}

TEST (DBFile, read)
{	int findx = 0;
	while (findx < 1 || findx > 7) {
		cout << "\n select table: \n";
		cout << "\t 1. nation \n";
		cout << "\t 2. region \n";
		cout << "\t 3. customer \n";
		cout << "\t 4. part \n";
		cout << "\t 5. partsupp \n";
		cout << "\t 6. orders \n";
		cout << "\t 7. lineitem \n \t ";
		cin >> findx;
	}

	Record temp;
	DBFile dbfile;
	std::cout<<findx<<"\n";;
	dbfile.Open(rel_ptr[findx-1]->path());
	//CNF cnf;
	//Record literal;
	int counter=0;
	while(dbfile.GetNext(temp))
	{
		//if(counter%100000==0)
			temp.Print(rel_ptr[findx-1]->schema());
		//counter++;
	}
	dbfile.Close();
}
TEST(CNF_TEST, first_test_case){
	for(auto i:rel_ptr){
		cout << " Filter with CNF for : " << i->name() << "\n";
		DBFile dbfile;
		dbfile.Open(i->path());
		dbfile.MoveFirst();
		Record temp;
		int counter;
		CNF cnf;
		Record literal;
		i->get_cnf (cnf, literal);
		while(dbfile.GetNext(temp,cnf,literal))
		{
				temp.Print (i->schema());
		}
		
	}
}
// sequential scan of a DBfile 
/*
void test2 () {

	DBFile dbfile;
	dbfile.Open (rel->path());
	dbfile.MoveFirst ();

	Record temp;

	int counter = 0;
	while (dbfile.GetNext (temp) == 1) {
		counter += 1;
		temp.Print (rel->schema());
		if (counter % 10000 == 0) {
			cout << counter << "\n";
		}
	}
	cout << " scanned " << counter << " recs \n";
	dbfile.Close ();
}

// scan of a DBfile and apply a filter predicate
void test3 () {

	cout << " Filter with CNF for : " << rel->name() << "\n";

	CNF cnf; 
	Record literal;
	rel->get_cnf (cnf, literal);

	DBFile dbfile;
	dbfile.Open (rel->path());
	dbfile.MoveFirst ();

	Record temp;

	int counter = 0;
	while (dbfile.GetNext (temp, cnf, literal) == 1) {
		counter += 1;
		temp.Print (rel->schema());
		if (counter % 10000 == 0) {
			cout << counter << "\n";
		}
	}
	cout << " selected " << counter << " recs \n";
	dbfile.Close ();
}
*/
int main(int argc, char* argv[]){

	setup (catalog_path, dbfile_dir, tpch_dir.c_str());
	rel_ptr = {n, r, c, p, ps, o, li};
	//rel_ptr= {li};
    /*void (*test_ptr[]) () = {&test1, &test2, &test3};  

	int tindx = 0;
	while (tindx < 1 || tindx > 3) {
		cout << " select test: \n";
		cout << " \t 1. load file \n";
		cout << " \t 2. scan \n";
		cout << " \t 3. scan & filter \n \t ";
		cin >> tindx;
	}

	int findx = 0;
	while (findx < 1 || findx > 7) {
		cout << "\n select table: \n";
		cout << "\t 1. nation \n";
		cout << "\t 2. region \n";
		cout << "\t 3. customer \n";
		cout << "\t 4. part \n";
		cout << "\t 5. partsupp \n";
		cout << "\t 6. orders \n";
		cout << "\t 7. lineitem \n \t ";
		cin >> findx;
	}

	rel = rel_ptr [findx - 1];
	test = test_ptr [tindx - 1];

	test ();*/

	//cleanup ();
    ::testing::InitGoogleTest(&argc,argv);
    testing::internal::CaptureStdout();
    std::cout << "My test";
    std::string output = testing::internal::GetCapturedStdout();
    return RUN_ALL_TESTS();
}