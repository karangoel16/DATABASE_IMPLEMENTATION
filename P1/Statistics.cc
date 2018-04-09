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
    for(auto iter:relation[oldRelation].attr)
        relation[newRelation].attr[newRelation+"."+iter.first]=iter.second;
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
	struct AndList *pA = parseTree;
    double fact = 1.0,orFact = 0,orFactDe = 0,orFactPartial = 0;
    while(pA!= NULL){
		struct OrList *pOr = pA->left;
		orFact = 0;
		orFactDe = 1;
		unordered_map<string, int> diff;
		string previous;
		while(pOr!=NULL){
				struct ComparisonOp *pComp = pOr->left;
				double tLeft = 1.0,tRight = 1.0;
				if(!checkAtt(pComp->left, tLeft, relNames, numToJoin) || !checkAtt(pComp->right, tRight, relNames, numToJoin)){
					return ;
				}			
				string oper1(pComp->left->value);

				if (pComp->code == 1 || pComp->code == 2){
						orFact += 1.0/3;
						orFactDe *= 1.0/3;
                        for(auto & iter:relation)
							if(iter.second.attr.count(oper1)!=0)
								iter.second.attr[oper1] /= 3;
						
						pOr = pOr->rightOr;
						continue;
                }
				if(tRight==1.0|| tLeft>tRight || tLeft<tRight){
					if(diff.count(oper1)){
						diff[oper1]++;
					}
					else{
						diff[oper1] = min(tLeft,tRight);
					}
						
                }
				if(oper1.compare(previous)!=0 && orFactPartial != 0){
						orFact += orFactPartial;
						orFactDe *= orFactPartial;
						orFactPartial = 0;
						
					}
					orFactPartial += 1.0/max(tLeft, tRight);
					previous = oper1;

				pOr = pOr->rightOr;
        }
        
       if(orFactPartial){
				orFact += orFactPartial;
				orFactDe *= orFactPartial;
				orFactPartial = 0;				
		}
        
		if(orFact!=orFactDe){
			fact *= (orFact - orFactDe);
		}
		else{
			fact *=  orFact;
		}
        for(auto &ch:diff)
            for(auto &rel:relation)
				if(rel.second.attr.count(ch.first)){
					rel.second.attr[ch.first] = ch.second;
				}
        pA = pA->rightAnd;
    }
  
    long double maxTup = 1;
    bool reltable[numToJoin];
    for(int i=0; i<numToJoin; i++){
		reltable[i] = true;
	}
	
    for(int i=0; i<numToJoin; i++){
		if(!reltable[i]){
			continue;
		}
		string relname(relNames[i]);
		for(auto iter = estimate.begin(); iter!=estimate.end(); iter++){
            auto check=split(iter->first);
			if(check.count(relNames[i])!=0){				
				reltable[i] = false;
				maxTup *= iter->second;
				for(int j=i+1; j<numToJoin; j++){
					if(check.count(relNames[j])!=0)
						reltable[j] = false;
				}
                break;
            }
        }
	 }
	 
	double result = fact * maxTup;

    string newSet;
    for(int i=0; i<numToJoin; i++){
		string relname(relNames[i]);
		newSet+=relname+"#";
		for(auto iter =estimate.begin(); iter!=estimate.end(); iter++){
            auto check=split(iter->first);
			if(check.count(relname)!=0){
				estimate.erase(iter);
				break;
			}
		}
    }
    estimate.insert({newSet,result});
}
double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
    if(!checkRel(relNames,numToJoin)){
        return 0;
    }
    struct AndList *pA = parseTree;
    double fact = 1.0;
    double orFact = 0;
    double orFactDe = 0;
    double orFactPartial = 0;
    while(pA!= NULL){
		struct OrList *pOr = pA->left;
		orFact = 0;
		orFactDe = 1;
		string previous;
		while(pOr!=NULL){
				struct ComparisonOp *pComp = pOr->left;
				double tLeft = 1.0,tRight = 1.0;
				if(!checkAtt(pComp->left, tLeft, relNames, numToJoin) || !checkAtt(pComp->right, tRight, relNames, numToJoin))
					return 0;
				if (pComp->code == 1 || pComp->code == 2){
					orFact += 1.0/3;
					orFactDe *= 1.0/3;
					pOr = pOr->rightOr;
						continue;
				}

				string oper1(pComp->left->value);
				if(oper1.compare(previous)!=0 && orFactPartial != 0){
						orFact += orFactPartial;
						orFactDe *= orFactPartial;
						orFactPartial = 0;						
				}
				orFactPartial += 1.0/max(tLeft, tRight);
				previous = oper1;
				pOr = pOr->rightOr;
        }
        if(orFactPartial != 0){
				orFact += orFactPartial;
				orFactDe *= orFactPartial;
				orFactPartial = 0;				
		}
		if(orFact!=orFactDe){
			fact *= (orFact - orFactDe);
		}
		else{
			fact *=  orFact;
		}
        pA = pA->rightAnd;
    }
  
    long double maxTup = 1;
    vector<bool> reltable(numToJoin,true);
    for(int i=0; i<numToJoin; i++){
		if(!reltable[i]){
			continue;
		}
			
		string relname(relNames[i]);
		
		for(auto iter = estimate.begin(); iter!=estimate.end(); iter++){
            auto check=split(iter->first);
			if(check.count(relNames[i])){				
				reltable[i] = false;
				maxTup *= iter->second;
				
				for(int j=0; j<numToJoin; j++){
					if(check.count(relNames[j])){
						reltable[j] = false;
					}
						
				}
                break;
            }
        }
	 }
    return fact * maxTup;
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
    for(auto iter:stat.relation)
        os<<iter.first<<" "<<iter.second;
    os<<save_two_values<unordered_map<string,double>>(stat.estimate);
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
    is>>size;
    for(int i=0;i<size;i++){
        string temp;
        double dbl;
        is>>temp>>dbl;
        stat.estimate[temp]=dbl;
    }
    return is;
}