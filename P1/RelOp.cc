#include "RelOp.h"

/*template <class File,class Pipe,class Cnf,class Rec> 
struct Param4{
	File file;
	Pipe outPipe;
	Cnf Cf1;
	Rec record;
	//constructor overloading for the template structure defined here
	Param4(){};
	Param4(File inFile,Pipe oPipe,Cnf Cf,Rec literal):file(inFile),outPipe(oPipe),Cf1(Cf),record(literal){};
};*/
struct Param4{
	DBFile *dbfile;
	Pipe *outPipe;
	CNF *cnf;
	Record *literal;
	int i=100;
};

  
void SelectFile::Run (DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal) {
	Param4 *args=static_cast<struct Param4 *>(malloc(sizeof(struct Param4)));
	args->dbfile=&inFile;
	args->outPipe=&outPipe;
	args->cnf=&selOp;
	args->literal=&literal;
	create_join_thread(&worker,thread_work,(void *)args);
	outPipe.ShutDown();
}

void *SelectFile::thread_work(void *args){
  struct Param4 *arg = (struct Param4 *)(args);                     
  arg->dbfile->MoveFirst();
  Record next;
  while(arg->dbfile->GetNext(next,*arg->cnf,*arg->literal))
	arg->outPipe->Insert(&next);
  arg->outPipe->ShutDown();
}

int RelationalOp::create_join_thread(pthread_t *thread,void *(*start_routine) (void *), void *arg){
	//this is to create joinable thread
	pthread_attr_t attr;
  	pthread_attr_init(&attr);
  	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  	int rc = pthread_create(thread, &attr, start_routine, arg);
  	pthread_attr_destroy(&attr);
	return rc;
}
