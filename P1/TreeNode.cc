#include "TreeNode.h"

void Node::wait(){}

void SelectFNode::Print(){
    if(left)
        left->Print();
	cout <<"*----Select Operation : File----*"<<endl;
	cout <<endl;
	cout <<"Input DB File:	"<<dbfilePath<<endl;
	cout <<"Output Pipe:	"<<oPipe<<endl;
	cout <<"Output Schema:	" <<endl;
    outputSchema->Print();
	cout <<"Select CNF :	" <<endl;
	cout <<"\t"; cnf->Print();
	cout <<"\n\n";
	if(right)
		right->Print();
}

void SelectPNode::Print(){
	if(left)
		left->Print();
	cout <<"*----Select Operation : Pipe----*"<<endl;
	cout <<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe:	"<<oPipe<<endl;
	cout <<"Output Schema:	" <<endl;
    outputSchema->Print();
	cout <<"Select CNF:	" <<endl;
	cout <<"\t"; cnf->Print();
	cout <<"\n\n";
	if(right)
		right->Print();
}

void ProjectNode::Print(){
	if(left)
		left->Print();
	cout <<"*----Project Operation----*"<<endl;
	cout <<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe:	"<<oPipe<<endl;
	cout <<"Output Schema:	" <<endl;
    outputSchema->Print();
	cout <<"Attributes to keep:	"<<endl;
	cout <<"\t";
	for(int i=0;i<numAttsOutput;i++) {
		cout <<keepMe[i] <<", ";
	}
	cout <<endl;
	cout <<"\n";
	if(right)
		right->Print();
}

void JoinNode::Print(){
	if(left)
		left->Print();
	cout <<"*----Join Operation----*"<<endl;
	cout <<endl;
	cout <<"Left Input Pipe:	"<<lPipe<<endl;
	cout <<"Right Input Pipe:	"<<rPipe<<endl;
	cout <<"Output Pipe:	"<<oPipe<<endl;
	cout <<"Output Schema:	" <<endl;
    outputSchema->Print();
	cout <<"Select CNF:	" <<endl;
	cout <<"\t"; cnf->Print();
	cout <<"\n\n";
	if(right)
		right->Print();
}

void SumNode::Print(){
	if(left)
		left->Print();
	cout <<"*----Sum Operation----*"<<endl;
	cout <<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe:	"<<oPipe<<endl;
	cout <<"Output Schema:	" <<endl;
    outputSchema->Print();
	cout <<"Sum Function:	" <<endl;
		function->Print();
	cout <<endl;
	cout <<"\n";
	if(right)
		right->Print();
}

void GroupByNode::Print(){
	if(left)
		left->Print();
	cout <<"*----GroupBy Operation----*"<<endl;
	cout <<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe:	"<<oPipe<<endl;
	cout <<"Output Schema:	" <<endl;
    outputSchema->Print();
	cout <<"Group By OrderMaker:	" <<endl;
	order->Print();
	cout <<endl;
	cout <<"Group By Function:	" <<endl;
	function->Print();
	cout <<endl;
	cout <<"\n";
	if(right)
		right->Print();
}

void DistinctNode::Print(){
	if(left)
		left->Print();
	cout <<"*----Duplicate Removal Operation----*"<<endl;
	cout <<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe:	"<<oPipe<<endl;
	cout <<"Output Schema:	" <<endl;
    outputSchema->Print();
	cout <<"\n";
	if(right)
		right->Print();
}

void WriteOutNode::Print(){
	if(left)
		left->Print();
	cout <<"*----Write Out----*"<<endl;
	cout <<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Schema:	" <<endl;
    outputSchema->Print();
	cout <<"\n";
	if(right)
		right->Print();
}

void Node::Print(){
    if(left)
        left->Print();
    if(right)
        right->Print();
}


void Node::Execute(){
	NEED_TO_IMPLEMENT
}
void SelectFNode::Execute(){
	if(left!=NULL)
		left->Execute();
	if(right!=NULL)
		right->Execute();
	cout <<"execute selectfrom file: " <<dbfilePath<<endl;
	Pipe *sfOutPipe = new Pipe(PIPE_SIZE);
		//add it to the pipe
	pipe[oPipe] = sfOutPipe;
	DBFile *db=new DBFile();
	db->Open((char*)dbfilePath.c_str());
	sf->Run(*db, *sfOutPipe, *(cnf), *(literal));
	//operators.push_back(sf);
}

void SelectFNode::wait(){
	sf->WaitUntilDone();
}

void SelectPNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	//selectPipe->Use_n_Pages(RUNLEN);
	Pipe *spo = new Pipe(PIPE_SIZE);
	//add it to the pipe
	pipe[oPipe] = spo;
	Pipe *spl = pipe[lPipe];
	selectPipe->Run(*spl, *spo, *(cnf), *(literal));
	//operators.push_back(selectPipe);
}
void SelectPNode::wait(){
	selectPipe->WaitUntilDone();
}

void ProjectNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	std::cout<<"In Project Node\n";
	Pipe *pOutPipe = new Pipe(PIPE_SIZE);
	//add it to the pipe
	pipe[oPipe] = pOutPipe;
	Pipe *plPipe = pipe[lPipe];
	if(pOutPipe)
		std::cout<<keepMe[0]<<"YES\n";
	std::cout<<numAttsInput<<"\n";
	std::cout<<numAttsOutput<<endl;
	project->Run(*plPipe, *pOutPipe, keepMe, numAttsInput, numAttsOutput);
	//operators.push_back(project);
}

void ProjectNode::wait(){
	project->WaitUntilDone();
}

void JoinNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	Pipe *jOutPipe = new Pipe(PIPE_SIZE);
	//add it to the pipe
	pipe[oPipe] = jOutPipe;
	Pipe *jlPipe = pipe[lPipe];
	Pipe *jrPipe = pipe[rPipe];
	join->Run(*jlPipe, *jrPipe, *jOutPipe, *(cnf), *(literal));
	//operators.push_back(join);
}

void JoinNode::wait(){
	join->WaitUntilDone();
}

void SumNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	Pipe *sOutPipe = new Pipe(PIPE_SIZE);
	pipe[oPipe] = sOutPipe;
	Pipe *slPipe = pipe[lPipe];
	sum->Run(*slPipe, *sOutPipe, *(function));
	//operators.push_back(sum);
}

void SumNode::wait(){
	sum->WaitUntilDone();
}

void GroupByNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
//		groupBy->Use_n_Pages(RUNLEN);
	Pipe *gbOutPipe = new Pipe(PIPE_SIZE);
	pipe[oPipe] = gbOutPipe;
	Pipe *gblPipe = pipe[lPipe];
	groupBy->Run(*gblPipe, *gbOutPipe, *(order), *(function));
	//operators.push_back(groupBy);
}

void GroupByNode::wait(){
	groupBy->WaitUntilDone();
}

void DistinctNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
//		dr->Use_n_Pages(RUNLEN);
	Pipe *drOutPipe = new Pipe(PIPE_SIZE);
	pipe[oPipe] = drOutPipe;
	Pipe *drlPipe = pipe[lPipe];
	dr->Run(*drlPipe, *drOutPipe, *(left->outputSchema));
}

void DistinctNode::wait(){
	dr->WaitUntilDone();
}

void WriteOutNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	fp=fopen("hello.txt","w");
	if(!fp){
		std::cout<<"FILE CAN'T BE OPEN\n";
		return ;
	}
	Pipe *wlPipe = pipe[lPipe];
	std::mutex mtx; 
	mtx.lock();
	operators.push_back(wo);
	mtx.unlock();
	wo->Run(*wlPipe, fp, *(outputSchema));
	cout <<"total pipe size: " <<pipe.size()<<endl;
	wait();
	Record rec;
	fclose(fp);
}

void WriteOutNode::wait(){
	wo->WaitUntilDone();
}


