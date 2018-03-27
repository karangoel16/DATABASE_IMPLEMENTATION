Project Members:-

1. Karan Goel 10102301
2. Ashish Katiyar 48366895

//The rest of the read-me has been modified from initial readme given to us.

To run this module we need to create sorted dbfile or heapfile beforehand
Note that the driver only works with the tpch files (generated using the dbgen program). 

To compile the driver, type
	make a3.out
	make a2-test.out 
	make a1-test.out
	make a22.out

To run the driver, type
	1.	./a22.out or ./a1-test.out and save all the database for this driver to use
	2.	To run the driver, type 'test.out' followed by the query # you wish to execute
		./a3.out [1-8]

The Queries:
==============
1. select * from partsupp where ps_supplycost < 500
2. select p_partkey, p_name, p_retailprice from part where p_retailprice < 100;
3. select sum (s_acctbal + (s_acctbal * .05)) from supplier;
4. select sum (ps_supplycost) from supplier, partsupp where s_suppkey = ps_suppkey;
5. select distinct ps_suppkey from partsupp where ps_supplycost < 100.11;
6. select sum (ps_supplycost) from supplier, partsupp where s_suppkey = ps_suppkey groupby s_nationkey;
and follow the on-screen instructions.

Using the driver:
==================

1. SETTINGS: The following variables control the various file locations and they are declared in test.cat file:
	o dbfile_dir -- this is where the created heap db-files will be stored. By default, this is set to "" (thus all the heap dbfiles will be created in the current directory).
	o tpch_dir -- this stores the directory path where the tpch-files can be found.
        We prefer saving the version of 1Gb in /tmp folder.
	o catalog_path -- this stores the catalog file path. By default this is set to "".


