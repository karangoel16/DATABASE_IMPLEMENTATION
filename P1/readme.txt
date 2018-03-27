Project Members:-

1. Karan Goel 10102301
2. Ashish Katiyar 48366895

//The rest of the read-me has been modified from initial readme given to us.

To run this module we need to create sorted dbfile or heapfile beforehand
Note that the driver only works with the tpch files (generated using the dbgen program). 

To compile the driver, type
	make test.out
	make a2-test.out 
	make a1-test.out
	make a22.out

To run the driver, type
	1.	./a22.out or ./a1-test.out and save all the database for this driver to use
	2.	./test.out [1-8]
		for e.g. 
		./test.out 1

and follow the on-screen instructions.

Using the driver:
==================

1. SETTINGS: The following variables control the various file locations and they are declared in test.cat file:
	o dbfile_dir -- this is where the created heap db-files will be stored. By default, this is set to "" (thus all the heap dbfiles will be created in the current directory).
	o tpch_dir -- this stores the directory path where the tpch-files can be found.
        We prefer saving the version of 1Gb in /tmp folder.
	o catalog_path -- this stores the catalog file path. By default this is set to "".


