#include "TreeNode.h"

void SelectFNode::Print(){
    if(left!=nullptr)
        left->Print();
	cout <<"*****************"<<endl;
	cout <<"SelectFromFile Operation"<<endl;
	cout <<"Input File:	"<<dbfilePath<<endl;
	cout <<"Output Pipe: "<<oPipe<<endl;
	cout <<"Output Schema: " <<endl;
    outputSchema->Print();
	cout <<"Select CNF: " <<endl;
	cout <<"\t"; cnf->Print();
	cout <<"\n\n";
	if(right)
		right->Print();
}

void SelectPNode::Print(){
	if(left)
		left->Print();
	cout <<"*****************"<<endl;
	cout <<"SelectFromPipe Operation"<<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe: "<<oPipe<<endl;
	cout <<"Output Schema: " <<endl;
    outputSchema->Print();
	cout <<"Select CNF: " <<endl;
	cout <<"\t"; cnf->Print();
	cout <<"\n\n";
	if(right)
		right->Print();
}

void ProjectNode::Print(){
	if(left)
		left->Print();
	cout <<"*****************"<<endl;
	cout <<"Project Operation"<<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe: "<<oPipe<<endl;
	cout <<"Output Schema: " <<endl;
    outputSchema->Print();
	cout <<"Attributes to keep: "<<endl;
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
	cout <<"*****************"<<endl;
	cout <<"Join Operation"<<endl;
	cout <<"Left Input Pipe: "<<lPipe<<endl;
	cout <<"Right Input Pipe: "<<rPipe<<endl;
	cout <<"Output Pipe: "<<oPipe<<endl;
	cout <<"Output Schema: " <<endl;
    outputSchema->Print();
	cout <<"Select CNF: " <<endl;
	cout <<"\t"; cnf->Print();
	cout <<"\n\n";
	if(right)
		right->Print();
}

void SumNode::Print(){
	if(left)
		left->Print();
		cout <<"*****************"<<endl;
	cout <<"Sum Operation"<<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe: "<<oPipe<<endl;
	cout <<"Output Schema: " <<endl;
    outputSchema->Print();
	cout <<"Sum Function: " <<endl;
		function->Print();
	cout <<endl;
	cout <<"\n";
	if(right)
		right->Print();
}

void GroupByNode::Print(){
	if(left)
		left->Print();
	cout <<"*****************"<<endl;
	cout <<"GroupBy Operation"<<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe: "<<oPipe<<endl;
	cout <<"Output Schema: " <<endl;
    outputSchema->Print();
	cout <<"Group By OrderMaker: " <<endl;
	order->Print();
	cout <<endl;
	cout <<"Group By Function: " <<endl;
	function->Print();
	cout <<endl;
	cout <<"\n";
	if(right)
		right->Print();
}

void DistinctNode::Print(){
	if(left)
		left->Print();
	cout <<"*****************"<<endl;
	cout <<"Duplicate Removal Operation"<<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Pipe: "<<oPipe<<endl;
	cout <<"Output Schema: " <<endl;
    outputSchema->Print();
	cout <<"\n";
	if(right)
		right->Print();
}

void WriteOutNode::Print(){
	if(left)
		left->Print();
	cout <<"*****************"<<endl;
	cout <<"Write Out"<<endl;
	cout <<"Input Pipe:	"<<lPipe<<endl;
	cout <<"Output Schema: " <<endl;
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
	NEED_TO_IMPLEMENT
}

void SelectPNode::Execute(){
	NEED_TO_IMPLEMENT
}

void ProjectNode::Execute(){
	NEED_TO_IMPLEMENT
}

void JoinNode::Execute(){
	NEED_TO_IMPLEMENT
}

void SumNode::Execute(){
	NEED_TO_IMPLEMENT
}

void GroupByNode::Execute(){
	NEED_TO_IMPLEMENT
}

void DistinctNode::Execute(){
	NEED_TO_IMPLEMENT
}

void WriteOutNode::Execute(){
	NEED_TO_IMPLEMENT
}


