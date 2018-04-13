#include "Query.h"
void Query::ExecuteQuery(){
    Node *temp=new Node();
    temp->left=root;
    temp->lPipe = root->oPipe;
    temp->Execute();
    //Need to insert something here
}

void Query::PrintQuery(){
    if(root==nullptr){
        std::cout<<"The Tree is null\n";
        return ;
    }
    root->Print();
}

void Node::Print(){
    if(left)
        left->Print();
    switch(opType) {
	case SELECTF:
		cout <<"*****************"<<endl;
		cout <<"SelectFromFile Operation"<<endl;
		cout <<"Input File:	"<<dbfilePath<<endl;
		cout <<"Output Pipe: "<<oPipe<<endl;
		cout <<"Output Schema: " <<endl <<outputSchema<<endl;
		cout <<"Select CNF: " <<endl;
		cout <<"\t"; cnf->Print();
		cout <<"\n\n";
		break;
	case SELECTP:
		cout <<"*****************"<<endl;
		cout <<"SelectFromPipe Operation"<<endl;
		cout <<"Input Pipe:	"<<lPipe<<endl;
		cout <<"Output Pipe: "<<oPipe<<endl;
		cout <<"Output Schema: " <<endl<<outputSchema<<endl;
		cout <<"Select CNF: " <<endl;
		cout <<"\t"; cnf->Print();
		cout <<"\n\n";
		break;
	case PROJECT:
		cout <<"*****************"<<endl;
		cout <<"Project Operation"<<endl;
		cout <<"Input Pipe:	"<<lPipe<<endl;
		cout <<"Output Pipe: "<<oPipe<<endl;
		cout <<"Output Schema: " <<endl<<outputSchema<<endl;;
		cout <<"Attributes to keep: "<<endl;
		cout <<"\t";
		for(int i=0;i<numAttsOutput;i++) {
			cout <<keepMe[i] <<", ";
		}
		cout <<endl;
		cout <<"\n";
		break;
	case JOIN:
		cout <<"*****************"<<endl;
		cout <<"Join Operation"<<endl;
		cout <<"Left Input Pipe: "<<lPipe<<endl;
		cout <<"Right Input Pipe: "<<rPipe<<endl;
		cout <<"Output Pipe: "<<oPipe<<endl;
		cout <<"Output Schema: " <<endl<<outputSchema<<endl;
		cout <<"Select CNF: " <<endl;
		cout <<"\t"; cnf->Print();
		cout <<"\n\n";
		break;
	case SUM:
		cout <<"*****************"<<endl;
		cout <<"Sum Operation"<<endl;
		cout <<"Input Pipe:	"<<lPipe<<endl;
		cout <<"Output Pipe: "<<oPipe<<endl;
		cout <<"Output Schema: " <<endl<<outputSchema<<endl;
		cout <<"Sum Function: " <<endl;
		    function->Print();
		cout <<endl;
		cout <<"\n";
		break;
	case GROUPBY:
		cout <<"*****************"<<endl;
		cout <<"GroupBy Operation"<<endl;
		cout <<"Input Pipe:	"<<lPipe<<endl;
		cout <<"Output Pipe: "<<oPipe<<endl;
		cout <<"Output Schema: " <<endl<<outputSchema<<endl;
		cout <<"Group By OrderMaker: " <<endl;
		order->Print();
		cout <<endl;
		cout <<"Group By Function: " <<endl;
		function->Print();
		cout <<endl;
		cout <<"\n";
		break;
	case DISTINCT:
		cout <<"*****************"<<endl;
		cout <<"Duplicate Removal Operation"<<endl;
		cout <<"Input Pipe:	"<<lPipe<<endl;
		cout <<"Output Pipe: "<<oPipe<<endl;
		cout <<"Output Schema: " <<endl<<outputSchema<<endl;
		cout <<"\n";
		break;
	case WRITEOUT:
		cout <<"*****************"<<endl;
		cout <<"Write Out"<<endl;
		cout <<"Input Pipe:	"<<lPipe<<endl;
		cout <<"Output Schema: " <<endl<<outputSchema<<endl;
		cout <<"\n";
		break;
	default:
		break;
	}
    if(right)
        right->Print();
}

void Node::Execute(){
    if(left)
        left->Execute();
    if(right)
        right->Execute();
    switch(opType){
        case SELECTF:{
            SelectFile * sf=new SelectFile();
            outPipe = new Pipe(PIPE_SIZE);
            db=new DBFile();
            db->Open((char *)dbfilePath.c_str());
            db->MoveFirst();
            sf->Run(*db,*outPipe,*(cnf),*(literal));
            break;    
        };
        case SELECTP:
        {
            /*
            SelectPipe * sp=new SelectPipe();
            outPipe = new Pipe(PIPE_SIZE);
            sp->Run(*pipes[lPipe], *outPipe, *(node->cnf), *(node->literal));
            break;
            */
        };
    }
}