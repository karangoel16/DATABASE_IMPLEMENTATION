#include "Statistics.h"

Statistics::Statistics()
{
}
Statistics::Statistics(Statistics &copyMe)
{
}
Statistics::~Statistics()
{
}

void Statistics::AddRel(char *relName, int numTuples)
{
    relation[string(relName)].numAttr=numTuples;
}
void Statistics::AddAtt(char *relName, char *attName, int numDistincts)
{
    relation[string(relName)].AddAtt(string(attName),numDistincts);
}
void Statistics::CopyRel(char *oldName, char *newName)
{
    relation[string(newName)]=relation[string(oldName)];
}
	
void Statistics::Read(char *fromWhere)
{
    ifstream in(fromWhere);
    in>>*this;
    in.close();
}
void Statistics::Write(char *fromWhere)
{
    ofstream out(fromWhere);
    out<<*this;
    out.close();
}

void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin)
{
}
double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
}

std::ostream& operator<<(std::ostream& os, const Rel& relation){
    os<<relation.numAttr<<" "<<relation.attr.size()<<" ";
    for(auto i:relation.attr){
        os<<i.first<<" "<<i.second<<" ";
    }
    return os;
}

std::istream& operator>>(std::istream& is, Rel& Rel){
    int size;
    is>>Rel.numAttr>>size;
    for(int i=0;i<size;i++){
        string temp;
        is>>temp;
        is>>Rel.attr[temp];
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Statistics& stat){
    os<<stat.relation.size()<<" ";
    for(auto it:stat.relation){
        os<<it.first<<" "<<it.second;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Statistics& stat){
    int size;
    is>>size;
    for(int i=0;i<size;i++){
        string temp;
        struct Rel tempRel;
        is>>temp>>tempRel;
        stat.relation[temp]=tempRel;
    }
    return is;
}