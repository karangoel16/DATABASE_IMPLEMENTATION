#include "RelOp.h"


void sum(Pipe *inPipe,Pipe *outPipe, Function *func){
	Record rec;
	double result=0.0;
	while(inPipe->Remove(&rec)){
		int int_res=0;double dbl_res=0;
		func->Apply(rec,int_res,dbl_res);
		result += (int_res + dbl_res);
	}
	Attribute DA = {"double", Double};
	Schema sum_sch ("sum_sch", 1, &DA);
	stringstream ss;
	ss<<result<<"|";
	Record *rcd=new Record();
	rcd->ComposeRecord(&sum_sch, ss.str().c_str());
	outPipe->Insert(rcd);
	outPipe->ShutDown();
}
struct Param4{
	DBFile *dbfile;
	Pipe *outPipe;
	Pipe *inPipe,*inPipe2;
	CNF *cnf;
	Record *literal;
	Function *func;
	Schema *mySchema;
	int *keepMe;
	FILE *file;
	int numAttsInput;
	int numAttsOutput;
	OrderMaker *groupAtts;
};

  
void SelectFile::Run (DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal) {
	Param4 *args=static_cast<struct Param4 *>(malloc(sizeof(struct Param4)));
	args->dbfile=&inFile;
	args->outPipe=&outPipe;
	args->cnf=&selOp;
	args->literal=&literal;
	if(create_join_thread(&worker,thread_work,(void *)args)){
		std::cout<<"Some issue with thread creation here\n";
	}
}

void *SelectFile::thread_work(void *args){
  struct Param4 *arg = (struct Param4 *)(args);                     
  arg->dbfile->MoveFirst();
  Record next;
  int i=0;
  while(arg->dbfile->GetNext(next,*arg->cnf,*arg->literal))
		arg->outPipe->Insert(&next);
  arg->outPipe->ShutDown();
}

void SelectPipe::Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal){
	Param4 *args=static_cast<struct Param4 *>(malloc(sizeof(struct Param4)));
	args->inPipe=&inPipe;
	args->outPipe=&outPipe;
	args->cnf=&selOp;
	args->literal=&literal;
	if(create_join_thread(&worker,thread_work,(void *)args)){
		std::cout<<"Some issue with thread creation here\n";
	}
}

void* SelectPipe::thread_work(void* args){
	struct Param4 *arg = (struct Param4 *)(args);     
	ComparisonEngine cmp;
	Record rec;
	while(arg->inPipe->Remove(&rec))
		if(cmp.Compare(&rec,arg->literal,arg->cnf))
			arg->outPipe->Insert(&rec);
	arg->outPipe->ShutDown();
}

void Sum::Run (Pipe &inPipe, Pipe &outPipe, Function &computeMe){
	Param4 *args=static_cast<struct Param4 *>(malloc(sizeof(struct Param4)));
	args->inPipe=&inPipe;
	args->outPipe=&outPipe;
	args->func=&computeMe;
	if(create_join_thread(&worker,thread_work,(void *)args)){
		std::cout<<"Some issue with thread creation here\n";
	}
}

void *Sum::thread_work(void *args){
	struct Param4 *arg = (struct Param4 *)(args);   
	sum(arg->inPipe,arg->outPipe,arg->func);
}

void DuplicateRemoval::Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema) {
	Param4 *args=static_cast<struct Param4 *>(malloc(sizeof(struct Param4)));
	args->inPipe=&inPipe;
	args->outPipe=&outPipe;
	args->mySchema=&mySchema;
	if(create_join_thread(&worker,thread_work,(void *)args)){
		std::cout<<"Some issue with thread creation here\n";
	}
 }

//Duplicate needs to be done
void* DuplicateRemoval::thread_work(void* args){
	struct Param4 *arg = (struct Param4 *)(args);  
 	OrderMaker sortOrder(arg->mySchema);
  	Pipe sorted(100);
  	BigQ biq(*arg->inPipe, sorted, sortOrder, RUNLEN);
  	Record cur, next;
  	ComparisonEngine cmp;
	int c=0;
  	if(sorted.Remove(&cur)) {
    	while(sorted.Remove(&next))
      		if(cmp.Compare(&cur, &next, &sortOrder)) {
        		arg->outPipe->Insert(&cur);
				std::cout<<c++<<endl;
        		cur.Consume(&next);
      		}
    	arg->outPipe->Insert(&cur);
  	}
  	arg->outPipe->ShutDown();
}

int RelationalOp::create_join_thread(pthread_t *thread,void *(*start_routine) (void *), void *arg){
	//this is to create joinable thread
  	int rc = pthread_create(thread, NULL, start_routine, arg);
	return rc;
}


void Project::Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput){
	Param4 *args=static_cast<struct Param4 *>(malloc(sizeof(struct Param4)));
	args->inPipe=&inPipe;
	args->outPipe=&outPipe;
	args->keepMe=keepMe;
	args->numAttsInput=numAttsInput;
	args->numAttsOutput=numAttsOutput;
	if(create_join_thread(&worker,thread_work,(void *)args)){
		std::cout<<"Some issue with thread creation here\n";
	}
 }

 void* Project::thread_work(void* args){
	struct Param4 *arg = (struct Param4 *)(args);  
	Record tmpRcd;
	while(arg->inPipe->Remove(&tmpRcd)) {
		tmpRcd.Project(arg->keepMe, arg->numAttsOutput, arg->numAttsInput);
		arg->outPipe->Insert(&tmpRcd);
	}
	arg->outPipe->ShutDown();
	return NULL;
 }

 void GroupBy::Run (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe) {
	Param4 *args=static_cast<struct Param4 *>(malloc(sizeof(struct Param4)));
	args->inPipe = &inPipe;
	args->outPipe = &outPipe;
	args->groupAtts = &groupAtts;
	args->func = &computeMe;
	create_join_thread(&worker,thread_work,(void *)args);
}

void* GroupBy::thread_work(void* args){
	struct Param4 *arg = (struct Param4 *)(args); 
	Pipe sortPipe(100);
	BigQ *bigQ = new BigQ(*(arg->inPipe), sortPipe, *(arg->groupAtts), RUNLEN);
	int ir;  double dr;
	Attribute DA = {"double", Double};
	Schema sum_sch ("sum_sch", 1, &DA);
	int numAttsToKeep = arg->groupAtts->numAtts + 1;
	int *attsToKeep = new int[numAttsToKeep];
	attsToKeep[0] = 0; 
	for(int i=1;i<=numAttsToKeep;i++)
		attsToKeep[i]=arg->groupAtts->whichAtts[i-1];
	ComparisonEngine cmp;
	Record tmpRcd;
	while(sortPipe.Remove(&tmpRcd)){
		bool flag=true;
		while(flag){
			flag=false;
			double sum=0;
			arg->func->Apply(tmpRcd, ir, dr);
			sum+=(ir+dr);
			Record *r = new Record();
			Record *lastRcd = new Record;
			lastRcd->Copy(&tmpRcd);
			while(sortPipe.Remove(r)) {
				if(cmp.Compare(lastRcd, r, arg->groupAtts) == 0){
					arg->func->Apply(*r, ir, dr);
					sum += (ir+dr);
				} 
				else {
					tmpRcd.Copy(r);
					flag = true;
					break;
				}
			}
			stringstream ss;
			ss <<sum<<"|";
			Record sumRcd;
			sumRcd.ComposeRecord(&sum_sch, ss.str().c_str());
			Record *tuple = new Record();
			tuple->MergeRecords(&sumRcd, lastRcd, 1, arg->groupAtts->numAtts, attsToKeep,  numAttsToKeep, 1);
			arg->outPipe->Insert(tuple);
		}
	}
	arg->outPipe->ShutDown();
}

void WriteOut::Run (Pipe &inPipe, FILE *outFile, Schema &mySchema){
	Param4 *args=static_cast<struct Param4 *>(malloc(sizeof(struct Param4)));
	args->inPipe = &inPipe;
	args->file = outFile;
	args->mySchema = &mySchema;
	if(create_join_thread(&worker,thread_work,(void *)args)){
		std::cout<<"Some issue with thread creation here\n";
	}
}

void* WriteOut::thread_work(void* args){
	struct Param4 *arg = (struct Param4 *)(args); 
	Attribute *atts = arg->mySchema->GetAtts();
	int n = arg->mySchema->GetNumAtts();
	Record rec;
	int cnt=1;
	while(arg->inPipe->Remove(&rec)){
		fprintf(arg->file, "%d: ", cnt++);
		char *bits = rec.bits;
		for (int i = 0; i < n; i++) {
			fprintf(arg->file, "%s",atts[i].name);
			int pointer = ((int *) bits)[i + 1];
			fprintf(arg->file, "[");
			if (atts[i].myType == Int) {
				int *myInt = (int *) &(bits[pointer]);
				fprintf(arg->file, "%d",*myInt);
			} else if (atts[i].myType == Double) {
				double *myDouble = (double *) &(bits[pointer]);
				fprintf(arg->file, "%f", *myDouble);
			} else if (atts[i].myType == String) {
				char *myString = (char *) &(bits[pointer]);
				fprintf(arg->file, "%s", myString);
			}
			fprintf(arg->file, "]");
			if (i != n - 1) {
				fprintf(arg->file, ", ");
			}
		}
		fprintf(arg->file, "\n");
	}
}

void Join::Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal){
	Param4 *args=static_cast<struct Param4 *>(malloc(sizeof(struct Param4)));
	args->inPipe = &inPipeL;
	args->inPipe2 = &inPipeR;
	args->cnf=&selOp;
	args->literal=&literal;
	args->outPipe=&outPipe;
	if(create_join_thread(&worker,thread_work,(void *)args)){
		std::cout<<"Some issue with thread creation here\n";
	}
}

 void* Join::thread_work(void* args){
	struct Param4 *arg = (struct Param4 *)(args); 
	OrderMaker orderL;
	OrderMaker orderR;
	arg->cnf->GetSortOrders(orderL, orderR);
	if(orderL.numAtts && orderR.numAtts && orderL.numAtts == orderR.numAtts) {
		Pipe pipeL(100), pipeR(100);
		BigQ *bigQL = new BigQ(*(arg->inPipe), pipeL, orderL, RUNLEN);
		BigQ *bigQR = new BigQ(*(arg->inPipe2), pipeR, orderR, RUNLEN);
		vector<Record *> vectorL;
		vector<Record *> vectorR;
	}
	arg->outPipe->ShutDown();
 }

