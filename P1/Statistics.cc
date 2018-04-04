#include "Statistics.h"
#include "Helper.h"
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
    estimate[string(relName)]=numTuples;
}
void Statistics::AddAtt(char *relName, char *attName, int numDistincts)
{
    relation[string(relName)].AddAtt(string(attName),numDistincts);
}
void Statistics::CopyRel(char *oldName, char *newName)
{
    string newRelation=string(newName),oldRelation=string(oldName);
    relation[string(newName)].numAttr=relation[oldRelation].numAttr;
    for(auto it:relation[oldRelation].attr)
        relation[newRelation].attr[newRelation+"."+it.first]=it.second;
    estimate[newRelation]=relation[newRelation].numAttr;
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
    if(!checkRel( relNames, numToJoin))
		return ;	
	struct AndList *pAnd = parseTree;
    double factorial = 1.0;
    double Or_fact = 0;
    double Or_fact_de = 0;
    double Or_fact_partial = 0;
    while(pAnd!= NULL){
		struct OrList *pOr = pAnd->left;
		Or_fact = 0;
		Or_fact_de = 1;
		unordered_map<string, int> change;
		string previous;
		while(pOr!=NULL){
				struct ComparisonOp *pComp = pOr->left;
				double t1 = 1.0;
				double t2 = 1.0;
				if(!checkAttributes(pComp->left, t1, relNames, numToJoin) || !checkAttributes(pComp->right, t2, relNames, numToJoin))
					return ;
				string operand1(pComp->left->value);
				if (pComp->code == 1 || pComp->code == 2){
						Or_fact += 1.0/3;
						Or_fact_de *= 1.0/3;
                        for(auto & it:relation)
							if(it.second.attr.count(operand1)!=0)
								it.second.attr[operand1] /= 3;
						
						pOr = pOr->rightOr;
						continue;
                }
				if(t2==1.0|| t1>t2 || t1<t2){
					if(change.count(operand1))
						change[operand1]++;
					else
						change[operand1] = min(t1,t2);
                }
				if(operand1.compare(previous)!=0 && Or_fact_partial != 0){
						Or_fact += Or_fact_partial;
						Or_fact_de *= Or_fact_partial;
						Or_fact_partial = 0;
						
					}
					Or_fact_partial += 1.0/max(t1, t2);
					previous = operand1;

				pOr = pOr->rightOr;
        }
        
       if(Or_fact_partial){
				Or_fact += Or_fact_partial;
				Or_fact_de *= Or_fact_partial;
				Or_fact_partial = 0;				
		}
        
		if(Or_fact!=Or_fact_de)
			factorial *= (Or_fact - Or_fact_de);
		else
			factorial *=  Or_fact;
        for(auto &ch:change)
            for(auto &rel:relation)
				if(rel.second.attr.count(ch.first)){
					rel.second.attr[ch.first] = ch.second;
				}
        pAnd = pAnd->rightAnd;
    }
  
    long long maxTuples = 1;
    bool reltable[numToJoin];
    for(int i=0; i<numToJoin; i++){
		reltable[i] = true;
	}
	
    for(int i=0; i<numToJoin; i++){
		if(!reltable[i])
			continue;
		string relname(relNames[i]);
		for(auto it = estimate.begin(); it!=estimate.end(); it++){
            auto check=split(it->first);
			if(check.count(relNames[i])!=0){				
				reltable[i] = false;
				maxTuples *= it->second;
				for(int j=i+1; j<numToJoin; j++){
					if(check.count(relNames[j])!=0)
						reltable[j] = false;
				}
                break;
            }
        }
	 }
	 
	double res = factorial * maxTuples;
	cout<<"Apply result "<<res<<endl;

    string newSet;
    for(int i=0; i<numToJoin; i++){
		string relname(relNames[i]);
		newSet+=relname+"#";
		for(auto it =estimate.begin(); it!=estimate.end(); it++){
            auto check=split(it->first);
			if(check.count(relname)!=0){
				estimate.erase(it);
				break;
			}
		}
    }
    estimate.insert({newSet,res});
}
double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
    if(!checkRel(relNames,numToJoin)){
        return 0;
    }
    struct AndList *pAnd = parseTree;
    double factorial = 1.0;
    double Or_fact = 0;
    double Or_fact_de = 0;
    double Or_fact_partial = 0;
    while(pAnd!= NULL){
		struct OrList *pOr = pAnd->left;
		Or_fact = 0;
		Or_fact_de = 1;
		string previous;
		while(pOr!=NULL){
				struct ComparisonOp *pComp = pOr->left;
				double t1 = 1.0;
				double t2 = 1.0;
				if(!checkAttributes(pComp->left, t1, relNames, numToJoin) || !checkAttributes(pComp->right, t2, relNames, numToJoin))
					return 0;
				if (pComp->code == 1 || pComp->code == 2){
					Or_fact += 1.0/3;
					Or_fact_de *= 1.0/3;
					pOr = pOr->rightOr;
						continue;
				}

				string operand1(pComp->left->value);
				if(operand1.compare(previous)!=0 && Or_fact_partial != 0){
						Or_fact += Or_fact_partial;
						Or_fact_de *= Or_fact_partial;
						Or_fact_partial = 0;						
				}
				Or_fact_partial += 1.0/max(t1, t2);
				previous = operand1;
				pOr = pOr->rightOr;
        }
        if(Or_fact_partial != 0){
				Or_fact += Or_fact_partial;
				Or_fact_de *= Or_fact_partial;
				Or_fact_partial = 0;				
				
		}
		if(Or_fact!=Or_fact_de)
			factorial *= (Or_fact - Or_fact_de);
		else
			factorial *=  Or_fact;

        pAnd = pAnd->rightAnd;
    }
  
    long long maxTuples = 1;
    vector<bool> reltable(numToJoin,true);
    for(int i=0; i<numToJoin; i++){
		if(!reltable[i])
			continue;
		string relname(relNames[i]);
		
		for(auto it = estimate.begin(); it!=estimate.end(); it++){
            auto check=split(it->first);
			if(check.count(relNames[i])){				
				reltable[i] = false;
				maxTuples *= it->second;
				
				for(int j=0; j<numToJoin; j++){
					if(check.count(relNames[j]))
						reltable[j] = false;
				}
                break;
            }
        }
	 }
	cout<<"Estimate Result: "<<factorial * maxTuples<<endl;
    return factorial * maxTuples;
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
    for(auto it:stat.relation)
        os<<it.first<<" "<<it.second;
    //os<<save_two_values<unordered_map<string,double>>(stat.estimate);
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
        //stat.estimate[temp]=tempRel.numAttr;
    }
    /*is>>size;
    for(int i=0;i<size;i++){
        string temp;
        double dbl;
        is>>temp>>dbl;
        stat.estimate[temp]=dbl;
    }*/
    return is;
}