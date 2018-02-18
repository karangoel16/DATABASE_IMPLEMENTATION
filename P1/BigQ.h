#ifndef BIGQ_H
#define BIGQ_H
#pragma once
#include <pthread.h>
#include <iostream>
#include "Pipe.h"
#include "File.h"
#include "Record.h"
#include <vector>
#include <stdio.h>
#include "ComparisonEngine.h"
#include <algorithm>
#include "DBFile.h"
#include <string>
#include <queue>

class DBFile; 

using namespace std;

class BigQ {
	Pipe &Pin, &Pout;
	pthread_t threadRes;
	DBFile * dbfile;
	OrderMaker &myOrder;
	int runLength;
	string tempFile;
	File file;
	/*
		USER_DEFINED RANDOM FUNCTION
		Generation of pseudo Random class
		https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
	*/
	std::string random_string( size_t length )
	{
		auto randchar = []() -> char
		{
			const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[ rand() % max_index ];
		};
		std::string str(length,0);
		std::generate_n( str.begin(), length, randchar );
		return str;
	}
public:

	BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
	~BigQ ();
	//this function will remain same for everyone
	static void * result(void *bigQ);
};




#endif
