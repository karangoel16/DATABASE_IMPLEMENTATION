Project Members:-

1. Karan Goel 10102301
2. Ashish Katiyar 48366895

//The rest of the read-me has been modified from initial readme given to us.

This test driver gives a menu-based interface to three options that allows you to test your code:
	1. load (read a tpch file and write it out a heap DBFile)
	2. scan (read records from an existing heap DBFile)
	3. scan & filter (read records and filter using a CNF predicate)

Note that the driver only works with the tpch files (generated using the dbgen program). 

To compile the driver, type
	make test.out

To run the driver, type
	test.out
and follow the on-screen instructions.

Using the driver:
==================

1. SETTINGS: The following variables control the various file locations and they are declared in test.cc (just after the #include header declarations):
	o dbfile_dir -- this is where the created heap db-files will be stored. By default, this is set to "" (thus all the heap dbfiles will be created in the current directory).
	o tpch_dir -- this stores the directory path where the tpch-files can be found.
        We prefer saving the version of 1Gb in /tmp folder.
	o catalog_path -- this stores the catalog file path. By default this is set to "".

2. Once load of a file has been selected, you can select option 2 or 3 to scan/filter all the records from the heap DBfile.  If option 3 is selected, a CNF should be supplied. Some example CNF's are given below. They are numbered q1,q2..q12. Use the table below to identify the tpch file associated with each CNF.
     	table    |   CNF
 ---------------------------------------
        region    |  q1 q2   
        nation    |  q3   
        supplier  |  q4 q5
        customer  |  q6
        part      |  q7   
        partsupp  |  q8 q9
        orders    |  q10                
        lineitem  |  q11 q12 

The expected output for these CNF's can be found in the file "output.log"

Example CNF's
================

q1 
(r_name = 'EUROPE')

q2 
(r_name < 'middle east') AND
(r_regionkey > 1)

q3 
(n_regionkey = 3) AND
(n_nationkey > 10) AND
(n_name > 'JAPAN')

q4 
(s_suppkey < 10)

q5
(s_nationkey = 18) AND
(s_acctbal > 1000) AND
(s_suppkey < 400)

q6
(c_nationkey = 23) AND
(c_mktsegment = 'FURNITURE') AND
(c_acctbal > 7023.99) AND
(c_acctbal < 7110.83)


q7 
(p_brand = 'Brand#13') AND
(p_retailprice > 500) AND
(p_retailprice < 930) AND
(p_size > 28) AND
(p_size < 1000000)

q8 
(ps_supplycost > 999.98)

q9 
(ps_availqty < 10)
(ps_supplycost > 100) AND
(ps_suppkey < 300) AND

q10 
(o_orderpriority = '1-URGENT') AND
(o_orderstatus = 'O') AND
(o_shippriority = 0) AND
(o_totalprice > 1015.68) AND
(o_totalprice < 1051.89)

q11
(l_shipdate > '1994-01-01') AND
(l_shipdate < '1994-01-04') AND
(l_discount > 0.049) AND
(l_discount < 0.061) AND
(l_quantity = 4.0)


q12
(l_orderkey > 100) AND
(l_orderkey < 1000) AND
(l_partkey > 100) AND
(l_partkey < 5000) AND
(l_shipmode = 'AIR') AND
(l_linestatus = 'F') AND
(l_tax < 0.07)
