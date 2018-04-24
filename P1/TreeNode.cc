#include "TreeNode.h"

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
	Pipe *sfOutPipe = new Pipe(PIPE_SIZE);
		//add it to the pipe
	SelectFile *sf=new SelectFile();
	pipe[oPipe] = sfOutPipe;
	DBFile *db=new DBFile();
	db->Open((char*)dbfilePath.c_str());
	db->MoveFirst();
	literal=new Record();
	sf->Run(*db, *sfOutPipe, *(cnf), *(literal));
	operators.push_back(sf);
}

void SelectPNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	SelectPipe *selectPipe = new SelectPipe();
	selectPipe->Use_n_Pages(RUNLEN);
	Pipe *spo = new Pipe(PIPE_SIZE);
	//add it to the pipe
	pipe[oPipe] = new Pipe(PIPE_SIZE);
	Pipe *spl = pipe[lPipe];
	selectPipe->Run(*spl, *spo, *(cnf), *(literal));
}

void ProjectNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	Project *project = new Project();
	Pipe *pOutPipe = new Pipe(PIPE_SIZE);
	//add it to the pipe
	pipe[oPipe] = pOutPipe;
	Pipe *plPipe = pipe[lPipe];
	project->Run(*plPipe, *pOutPipe, keepMe, numAttsInput, numAttsOutput);
	operators.push_back(project);
}

void JoinNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	Join *join = new Join;
	Pipe *jOutPipe = new Pipe(PIPE_SIZE);
	//add it to the pipe
	pipe[oPipe] = jOutPipe;
	Pipe *jlPipe = pipe[lPipe];
	Pipe *jrPipe = pipe[rPipe];
	join->Run(*jlPipe, *jrPipe, *jOutPipe, *(cnf), *(literal));
}

void SumNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	Sum *sum = new Sum;
	Pipe *sOutPipe = new Pipe(PIPE_SIZE);
	pipe[oPipe] = sOutPipe;
	Pipe *slPipe = pipe[lPipe];
	sum->Run(*slPipe, *sOutPipe, *(function));
}

void GroupByNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	GroupBy *groupBy = new GroupBy;
//		groupBy->Use_n_Pages(RUNLEN);
	Pipe *gbOutPipe = new Pipe(PIPE_SIZE);
	pipe[oPipe] = gbOutPipe;
	Pipe *gblPipe = pipe[lPipe];
	groupBy->Run(*gblPipe, *gbOutPipe, *(order), *(function));
}

void DistinctNode::Execute(){
	if(left)
		left->Execute();
	if(right)
		right->Execute();
	DuplicateRemoval *dr = new DuplicateRemoval;
//		dr->Use_n_Pages(RUNLEN);
	Pipe *drOutPipe = new Pipe(PIPE_SIZE);
	pipe[oPipe] = drOutPipe;
	Pipe *drlPipe = pipe[lPipe];
	dr->Run(*drlPipe, *drOutPipe, *(left->outputSchema));
}

void WriteOutNode::Execute(){
	WriteOut *wo = new WriteOut;
//		wo->Use_n_Pages(RUNLEN);
	Pipe *wlPipe = pipe[lPipe];
	wo->Run(*wlPipe, nullptr, *(outputSchema));
//		cout <<"total pipe size: " <<this->pipes.size()<<endl;
	operators.push_back(wo);
}


