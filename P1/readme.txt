Project Members:-

1. Karan Goel 10102301
2. Ashish Katiyar 48366895

//The rest of the read-me has been modified from initial readme given to us.

This test driver gives a menu-based interface to three options that allows you to test your code:
         1. create sorted dbfile
         2. scan a dbfile
         3. run some query

Note that the driver only works with the tpch files (generated using the dbgen program). 

To compile the driver, type
	make a22.out

To run the driver, type
	./a22.out
and follow the on-screen instructions.

Using the driver:
==================

1. SETTINGS: The following variables control the various file locations and they are declared in test.cc (just after the #include header declarations):
	o dbfile_dir -- this is where the created heap db-files will be stored. By default, this is set to "" (thus all the heap dbfiles will be created in the current directory).
	o tpch_dir -- this stores the directory path where the tpch-files can be found.
        We prefer saving the version of 1Gb in /tmp folder.
	o catalog_path -- this stores the catalog file path. By default this is set to "".
