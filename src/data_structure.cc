/**
 * @file data_structure.cc
 *
 */

#include <stdio.h>
#include "data_structure.h"


void BoundaryCountArray::activate(const std::vector<std::vector<Vertex>> &DAG, Vertex v){
    for(int k=0;k<DAG[v].size();k++){
        this->count(DAG[v][k]);
    }
    return;
}

void BoundaryCountArray::inactivate(const std::vector<std::vector<Vertex>> &DAG, Vertex v){
    for(int k=0;k<DAG[v].size();k++){
        this->uncount(DAG[v][k]);
    }
    return;
}

SelectedArray::SelectedArray(size_t len_query, const Graph &data){
    
    this->arr = new Vertex[len_query];

    this->len_query = len_query;

    for (size_t i=0;i<len_query;i++){
        arr[i]=-1;
    }

    size_t len = data.GetNumVertices();
    this->check = new bool[len];
    for (size_t i=0;i<len;i++){
            check[i]=false;
    }

    this->result_count = 0;

    return;
}



void SearchStackElement::print(){
    Vertex query_node = this->getQueryNode();
    Vertex selected_cs = this->getSelectedCS();
    std::cout<<"<Search stack element> query_node: "<< query_node << ", selected_cs: "<< selected_cs <<"\n";
    std::cout<<"next cs to search is ";
    
    int len = this->next_cs.size();
    for(int i=0;i<len;i++){
        std::cout<<(this->next_cs)[i]<<", ";
    }
    std::cout<<"\n";
}

void SearchStack::print(){
    std::cout<<"<Search Stack>\n";
    int len = this->stack.size();
    for(int i=0;i<len;i++){
        (this->stack[i]).print();
    }
    std::cout<<"\n";
}

void ActivatedBitArray::print(){
    
    std::cout<<"<Activated> ";
    size_t len = this->len_query;
    for(size_t i=0;i<len;i++){
        if(this->isActivated(i)){
            std::cout<<i<<", ";
        }
    }
    std::cout<<"\n";
    
}

void BoundaryCountArray::print(){

    std::cout<<"<Boundary Count>\n";

    size_t len = this->len_query;
    for(size_t i=0;i<len;i++){

        std::cout<<i<<": "<<this->getBoundaryCount((Vertex) i)<<", ";
    }
    std::cout<<"\n";

}

void IncomingNumberArray::print(){
    std::cout<<"<Incoming Number>\n";

    size_t len = this->len_query;
    for(size_t i=0;i<len;i++){
        std::cout<<i<<": "<<this->getIncomingNumber((Vertex) i)<<", ";
    }
    std::cout<<"\n";


}

void SelectedArray::print(){
    std::cout<<"<Selected Array>\n";

    int len = this->len_query;
    for(size_t i=0;i<len;i++){
        std::cout<<i<<": "<<this->getCs((Vertex) i)<<", ";
    }
    std::cout<<"\n";


}


void SelectedArray::print_result(){
    size_t len = this->len_query;
    if (this->result_count ==0){
        std::cout<<"t "<<len<<"\n";
    }

    std::cout<<"a";

    for (size_t i=0;i<len;i++){
        std::cout<<" "<<arr[i];
    }

    std::cout<<"\n";

    this->result_count++;

    return;
}


void SelectedArray::print_result(const string &file_name){
    
    if(this->result_count ==0){
        this->file = new std::ofstream(file_name);
    }
    
    
    if (!this->file->is_open()){
        std::cout << "Result file "<< file_name << " not found.\n";
        exit(EXIT_FAILURE);
    }

    size_t len = this->len_query;
    std::cout<<result_count<<"\n";
    if (this->result_count ==0){
        std::cout<<"t "<<len<<"\n";
         (*(this->file))<<"t "<<len<<"\n";
    }

    std::cout<<"a ";
    (*(this->file))<<this->result_count<<"a ";

    for (size_t i=0;i<len;i++){
        std::cout<<" "<<arr[i];
        (*(this->file))<<" "<<arr[i];
    }

    std::cout<<"\n";
    (*(this->file))<<"\n";

    this->result_count++;
    return;
}



void AvoidVectorElement::print(){
    std::cout<<"<Avoid Vector Element>\n";


    Vertex CS = this->getCS();
    Vertex Yield = this->getYield();
    Vertex Desperate = this->getDesperate();

    std::cout<<"CS: "<<CS<<", Yield: "<<Yield<<", Desperate: "<<Desperate<<"\n";

}

void AvoidVector::print(){
    std::cout<<"<Avoid vector>\n";

    size_t len = this->av.size();
    for(size_t i=0;i<len;i++){
        (this->av)[i].print();
    }

}



