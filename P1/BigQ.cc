#include "BigQ.h"

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen): Pin(in),Pout(out), myOrder(sortorder),runLength(runlen)  {
	tempFile=random_string(15)+".bin";
	file.Open(0,const_cast<char *>(tempFile.c_str()));
	int rs = pthread_create(&threadRes, NULL, result, (void *)this);
	if(rs){
		printf("Error creating worker thread! Return %d\n", rs);
		exit(-1);
	}
}

BigQ::~BigQ () {
}

void * BigQ::result(void *Big){
	BigQ *bigQ= (BigQ *)Big;
	auto comp = [&](Record *p,Record *q){
		ComparisonEngine comp;
		return comp.Compare(p,q,&bigQ->myOrder)<0?true:false;
	};

	auto comp1 = [&](pair<int,Record *>p,pair<int,Record *>q){
		ComparisonEngine comp;
		return comp.Compare(p.second,q.second,&bigQ->myOrder)<0?false:true;
	};
	
	Record temp;
	Page *writePage=new Page();
	size_t curLength=0;
	int numPages=0;
	vector<Record *> result; //this will be used to sort the values internally within reach run;
	vector<int> pageCounter;
	int count=0;
	while((bigQ->Pin).Remove(&temp)){
		char * temp_bits=temp.GetBits();
		if(curLength+((int *)temp_bits)[0] < (PAGE_SIZE)*(bigQ->runLength)){
			Record *rec = new Record();
			rec->Consume(&temp);
			result.push_back(rec);
			curLength += ((int *)temp_bits)[0];
		}
		else{
			std::sort(result.begin(),result.end(),comp);
			pageCounter.push_back(numPages);
			for(auto i:result)
			{	
				if(!writePage->Append(i))
    			{
        			int pos = bigQ->file.GetLength();
					pos=(pos==0?0:(pos-1)); 
        			bigQ->file.AddPage(writePage,pos);
        			writePage->EmptyItOut();
        			writePage->Append(i);
					numPages++;
    			}
			}
			//for(auto i:result)
			//	delete i;
			result.clear();
			Record *rec = new Record();
			rec->Consume(&temp);
			result.push_back(rec);
			curLength = ((int *)temp_bits)[0];
			//this is done to read one of the record from the file
		}
	}
	if(!result.empty()){
		//this is in case last record is still not empty we will write these records on file
		//bigQ->dbfile->getDBfile()->getFile().GetLength()==0?0:bigQ->dbfile->getDBfile()->getFile().GetLength()-1);
		std::sort(result.begin(),result.end(),comp);
		pageCounter.push_back(numPages);
		for(auto i:result){
			if(!writePage->Append(i))
			{
				int pos = bigQ->file.GetLength();
				pos=(pos==0?0:(pos-1)); 
				bigQ->file.AddPage(writePage,pos);
				writePage->EmptyItOut();
				writePage->Append(i);
				numPages++;
			}
		}
		if(!writePage->empty()){
			int pos = bigQ->file.GetLength();
			pos=(pos==0?0:(pos-1)); 
			bigQ->file.AddPage(writePage,pos);
			writePage->EmptyItOut();
			numPages++;
		}
		//for(auto i:result)
		//	delete i;
		result.clear();
	}
	pageCounter.push_back(numPages);
	std::cout<<pageCounter.size()<<"\n";
	vector<Page *> pageKeeper;
	priority_queue<pair<int, Record*>, vector<pair<int, Record*>>,decltype( comp1 ) > pq(comp1);
	for(int i=0;i<pageCounter.size()-1;i++ ){
		Page *temp_P = new Page();
		bigQ->file.GetPage(temp_P,pageCounter[i]);
		Record *temp_R = new Record();
 		temp_P->GetFirst(temp_R);
		pq.push(make_pair(i,temp_R));
		pageKeeper.push_back(temp_P);
	}
	vector<int> runcur(pageCounter);
	while(!pq.empty()){
		auto top=pq.top();
		bigQ->Pout.Insert(top.second);
		pq.pop();
		Record *temp_R=new Record();
		if(!pageKeeper[top.first]->GetFirst(temp_R)){
			if(++runcur[top.first]<pageCounter[top.first+1]){
				pageKeeper[top.first]->EmptyItOut();
 				bigQ->file.GetPage(pageKeeper[top.first], runcur[top.first]);
 				pageKeeper[top.first]->GetFirst(temp_R);
				pq.push(make_pair(top.first,temp_R));
 			}
		}
		else{
			pq.push(make_pair(top.first,temp_R));
		}
	}
	//for(auto i:pageKeeper)
	//	delete i;
	bigQ->file.Close();
	remove((bigQ->tempFile).c_str());
	remove((bigQ->tempFile+".meta").c_str());
	bigQ->Pout.ShutDown ();
}