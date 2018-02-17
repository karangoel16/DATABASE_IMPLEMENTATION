#include "BigQ.h"

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen): Pin(in),Pout(out), myOrder(sortorder),runLength(runlen)  {
	// read data from in pipe sort them into runlen pages
    // construct priority queue over sorted runs and dump sorted data 
 	// into the out pipe
	dbfile = new DBFile();
	tempFile=random_string(15)+".bin";
	dbfile->Create(tempFile.c_str(),heap,NULL);
	//this will be used to save database temp

	int rs = pthread_create(&threadRes, NULL, result, (void *)this);
    // finally shut down the out pipe
	
	if(rs){
		printf("Error creating worker thread! Return %d\n", rs);
		exit(-1);
	}
}

BigQ::~BigQ () {
}

void * BigQ::result(void *Big){
	BigQ *bigQ= (BigQ *)Big;
	Comparator comp(bigQ->myOrder);
	//Comparator1 comp2(bigQ->myOrder);
	Record temp;
	size_t curLength=0;
	vector<Record *> result; //this will be used to sort the values internally within reach run;
	vector<int> pageCounter;
	while((bigQ->Pin).Remove(&temp)){
		int temp_bits=sizeof(temp.GetBits());
		if(curLength+temp_bits<(PAGE_SIZE)*bigQ->runLength){
			Record *rec = new Record();
			rec->Consume(&temp);
			result.push_back(rec);
			curLength += temp_bits;
			continue;
		}
		else{
			std::sort(result.begin(),result.end(),comp);
			pageCounter.push_back(bigQ->dbfile->getDBfile()->getFile().GetLength()==0?0:bigQ->dbfile->getDBfile()->getFile().GetLength()-1);
			for(auto i:result)
			{	
				bigQ->dbfile->Add(*i);
				delete i;
			}
			result.clear();
			curLength = 0;
		}
	}
	if(result.size()){
		//this is in case last record is still not empty we will write these records on file
		pageCounter.push_back(bigQ->dbfile->getDBfile()->getFile().GetLength()==0?0:bigQ->dbfile->getDBfile()->getFile().GetLength()-1);
		std::sort(result.begin(),result.end(),comp);
		for(auto i:result){
			bigQ->dbfile->Add(*i);
			delete i;
		}
	}
	bigQ->dbfile->Close();
	bigQ->dbfile->Open(bigQ->tempFile.c_str());
	bigQ->dbfile->MoveFirst();
	auto comp1 = [&](pair<int,Record *>p,pair<int,Record *>q){
		ComparisonEngine comp;
		return comp.Compare(p.second,q.second,&bigQ->myOrder)<0?true:false;
	};
	vector<Page *> pageKeeper;
	vector<int> runcur(pageCounter);
	priority_queue<pair<int, Record*>, vector<pair<int, Record*>>,decltype( comp1 ) > pq(comp1);
	for(auto i:pageCounter){
		Page *temp_P = new Page();
		bigQ->dbfile->getDBfile()->getFile().GetPage(temp_P,i);
		Record *temp_R = new Record();
 		temp_P->GetFirst(temp_R);
		pq.push(make_pair(i,temp_R));
		pageKeeper.push_back(temp_P);
	}
	while(!pq.empty()){
		auto top=pq.top();
		bigQ->Pout.Insert(top.second);
		pq.pop();
		Record *temp_R=new Record();
		if(!pageKeeper[top.first]->GetFirst(temp_R)){
			if(top.first<bigQ->dbfile->getDBfile()->getFile().GetLength()-1 && ++runcur[top.first]<pageCounter[top.first+1]){
				pageKeeper[top.first]->EmptyItOut();
 				bigQ->dbfile->getDBfile()->getFile().GetPage(pageKeeper[top.first], runcur[top.first]);
 				pageKeeper[top.first]->GetFirst(temp_R);
 				pq.push(make_pair(top.first,temp_R));
 			}
		}
		else{
			pq.push(make_pair(top.first,temp_R));
		}
	}
	for(auto i:pageKeeper)
		delete i;
	bigQ->Pout.ShutDown ();
}