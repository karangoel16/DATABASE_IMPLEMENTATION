#ifndef STATISTICS_
#define STATISTICS_
#include "ParseTree.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
using namespace std;
struct Rel{
	int numAttr;
	unordered_map<string,int> attr;
	Rel(){};//default constructor
	Rel(int numAttr):numAttr(numAttr){}//this is default attribute
	void AddRel(char *attrName,int numDistincts)
	{
		attr[string(attrName)]=numDistincts;
	}
	void operator= (const Rel& copy){
		this->numAttr=copy.numAttr;
		this->attr=copy.attr;
	}
	void AddAtt(string Name,int val){
		attr[Name]=val;
	}
	friend std::ostream& operator<<(std::ostream& os, const Rel& relation);
  	friend std::istream& operator>>(std::istream& is, Rel& Rel);
};

class Statistics
{
	unordered_map<string,Rel> relation;
public:
	Statistics();
	Statistics(Statistics &copyMe);	 // Performs deep copy
	~Statistics();


	void AddRel(char *relName, int numTuples);
	void AddAtt(char *relName, char *attName, int numDistincts);
	void CopyRel(char *oldName, char *newName);
	
	void Read(char *fromWhere);
	void Write(char *fromWhere);

	void  Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
	double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);
	friend std::ostream& operator<<(std::ostream& os, const Statistics& stat);
  	friend std::istream& operator>>(std::istream& is, Statistics& stat);
};

#endif
