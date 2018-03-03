#include "SortedFile.h"
#include "BigQ.h"
#include <iostream>

using namespace std;

SortedFile::SortedFile(){
    myOrder=new OrderMaker();
    in=new Pipe(BUFFERSIZE),out=new Pipe(BUFFERSIZE);
}

void SortedFile::Add (Record &addme){
    in->Insert(&addme);
}

void SortedFile::setAttribute(OrderMaker *o,int run){
    myOrder=o;
    runLength=run;
    bigQ=new BigQ(*in,*out,*myOrder,runLength);
    int rs = pthread_create(&threadres, NULL, helper, (void *)this);
}

int SortedFile::Close () {
    in->ShutDown();
    pthread_join (threadres, NULL);
    check_write();
    file.Close();
    return 1;
}


int SortedFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
  OrderMaker queryorder, cnforder;
  OrderMaker::queryOrderMaker(*myOrder, cnf, queryorder, cnforder);
  ComparisonEngine cmp;
  if (!binarySearch(fetchme, queryorder, literal, cnforder, cmp)) return 0;
  do {
    if (cmp.Compare(&fetchme, &queryorder, &literal, &cnforder)) return 0;
    if (cmp.Compare(&fetchme, &literal, &cnf)) return 1;
  } while(VirtualDBFile::GetNext(fetchme));
  return 0;  
  return VirtualDBFile::GetNext(fetchme,cnf,literal);
};

int SortedFile::binarySearch(Record& fetchme, OrderMaker& queryorder, Record& literal, OrderMaker& cnforder, ComparisonEngine& cmp) {
  if (!VirtualDBFile::GetNext(fetchme)) 
    return 0;
  int result = cmp.Compare(&fetchme, &queryorder, &literal, &cnforder);
  if (result > 0) 
    return 0;
  else if (result == 0)
   return 1;
  int low=currentPage, high=file.GetLength()-2, mid=(low+high)/2;
  for (; low<mid; mid=(low+high)/2) {
    file.GetPage(&readPage, mid);
    if(!VirtualDBFile::GetNext(fetchme)){
            std::cout<<"Empty Page Found";
            return 0;
    } 
    result = cmp.Compare(&fetchme, &queryorder, &literal, &cnforder);
    if (result<0) low = mid;
    else if (result>0) high = mid-1;
    else high = mid;
  }
  file.GetPage(&readPage, low);
  do {  
    if (!VirtualDBFile::GetNext(fetchme)) return 0;
    result = cmp.Compare(&fetchme, &queryorder, &literal, &cnforder);
  } while (result<0);
  return result==0;
}