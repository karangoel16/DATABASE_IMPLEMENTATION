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
	Pipe *inPipe;
	CNF *cnf;
	Record *literal;
	Function *func;
	Schema *mySchema;
	int *keepMe;
	int numAttsInput;
	int numAttsOutput;
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
	create_join_thread(&worker,thread_work,(void *)args);
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
	create_join_thread(&worker,thread_work,(void *)args);
 }

//Duplicate needs to be done
void* DuplicateRemoval::thread_work(void* args){
	struct Param4 *arg = (struct Param4 *)(args);  
	Record rec;
	while(arg->inPipe->Remove(&rec)){
		char * temp_bits=rec.GetBits();
		std::cout<<(int *)temp_bits[0]<<"\n";
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
	create_join_thread(&worker,thread_work,(void *)args);
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