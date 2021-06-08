/**
 * @file data_structure.h
 *
 */

#ifndef DATA_STRUCTURE_
#define DATA_STRUCTURE_

#include "common.h"
#include <vector>
#include "graph.h"

using namespace std;


class SearchStackElement{

    public:

        SearchStackElement(){
            query_node = -1;
            selected_cs = -1;
            next_cs.clear();
            return;
        }

        SearchStackElement(Vertex query_node, Vertex selected_cs, vector<Vertex> &next_cs){
            this->query_node = query_node;
            this->selected_cs = selected_cs;
            this->next_cs.clear();
            this->next_cs.assign(next_cs.begin(),next_cs.end());
            return;
        }


        Vertex getQueryNode(){
            return query_node;
        }

        void setQueryNode(Vertex query_node){
            this->query_node = query_node;
            return; 
        }

        Vertex getSelectedCS(){
            return selected_cs;
        }

        void setSelectedCS(Vertex selected_cs){
            this->selected_cs = selected_cs;
            return;
        }

        bool isEmptyNextCS(){
            return next_cs.empty();
        }



        Vertex popNextCS(){
            Vertex temp = next_cs[0];
            next_cs.erase(next_cs.begin());
            return temp;
        }

        void pushNextCS(Vertex next_node){
            next_cs.emplace_back(next_node);
            return;
        }

        void print();

    private:

        Vertex query_node;
        Vertex selected_cs;
        vector<Vertex> next_cs;



};

class SearchStack{

    public:

        SearchStack(){
            stack.clear();
            return;
        }
        

        void push(SearchStackElement newE){
            stack.emplace_back(newE);
            return;
        }

        SearchStackElement pop(){
            SearchStackElement temp = stack.back();
            stack.pop_back();
            return temp;
        }

        bool isEmpty(){
            return stack.empty();
        }

        void print();

    private:
    
        vector<SearchStackElement> stack;


};

class ActivatedBitArray{

    public:

        ActivatedBitArray(size_t len_query){
            arr = new bool[len_query];
            this->len_query = len_query;
            for(size_t i = 0;i<len_query;i++){
                arr[i] = false;
            }
            return;
        }

        ~ActivatedBitArray(){
            delete[] arr;
            return;
        }

        bool isActivated(Vertex v){
            return arr[v];
        }

        void activate(Vertex v){
            arr[v] = true;
            return;
        }

        void inactivate(Vertex v){
            arr[v] = false;
            return;
        }

        void print();

    private:

        bool *arr;
        size_t len_query;

};


class BoundaryCountArray{

    public:
        BoundaryCountArray(size_t len_query){
            arr = new size_t[len_query];
            this-> len_query = len_query;
            for (size_t i = 0; i<len_query ; i++){
                arr[i] = 0;
            }
            return;
        }

        ~BoundaryCountArray(){
            delete[] arr;
            return;
        }

        size_t getBoundaryCount(Vertex v){
            return arr[v];
        }

        void activate(const vector<vector<Vertex>> &DAG, Vertex v);
        void inactivate(const vector<vector<Vertex>> &DAG, Vertex v);

        void print();

    private:

        size_t *arr;
        size_t len_query;

        void count(Vertex v){
            arr[v]++;
        }

        void uncount(Vertex v){
            arr[v]--;
        }

};


class IncomingNumberArray{

    public:
        IncomingNumberArray(size_t len_query){
            arr = new size_t[len_query];
            this-> len_query;
            for (size_t i=0;i<len_query;i++){
                arr[i]=0;
            }
            return;
        }

        ~IncomingNumberArray(){
            delete[] arr;
            return;
        }

        void count(Vertex v){
            arr[v]++;
            return;
        }

        size_t getIncomingNumber(Vertex v){
            return arr[v];
        }

        void print();

    private:
        size_t *arr;
        size_t len_query;

};


class SelectedArray{

    public:
        SelectedArray(size_t len_query, const Graph &data);

        ~SelectedArray(){
            delete[] arr;
            delete[] check;
            file->close();
        }

        void select(Vertex query_node, Vertex cs){
            arr[query_node] = cs;
            check[cs] = true;
            return;
        }

        void unselect(Vertex query_node){
            Vertex cs = arr[query_node];
            check[cs] = false;
            arr[query_node] = -1;
            return;
        }

        bool isSelected(Vertex cs){
            return check[cs];
        }

        bool isFull(){
            size_t len = sizeof(arr)/sizeof(arr[0]);

            for(size_t i=0; i<len;i++ ){
                if (arr[i]==-1){
                    return false;
                }
            }
            return true;
        }

        Vertex getSelectedOf(Vertex query_node) {
            return arr[query_node];
        }

        void print_result();
        void print_result(const string &file_name);
        
        void print();

    private:
        Vertex *arr;
        bool *check;
        int result_count;
        size_t len_query;
        std::ofstream* file;


        Vertex getCs(Vertex query_node){
            return arr[query_node];
        }



};


class AvoidVectorElement{
    
    public:
        AvoidVectorElement(){
            return;
        }

        AvoidVectorElement(Vertex cs, Vertex yield, Vertex desperate){
            this->cs = cs;
            this-> yield = yield;
            this-> desperate = desperate;
            return;
        }

        void setCs(Vertex cs) { 
            this->cs = cs;
            return;
        }
        void setYield(Vertex yield){
            this->yield = yield;
            return;
        }
        void setDesperate(Vertex Desperate){
            this->desperate = desperate;
            return;
        }

        Vertex getCS(){return cs;}
        Vertex getYield(){return yield; }
        Vertex getDesperate() {return desperate;}


        void print();


    private:
        Vertex cs;
        Vertex yield;
        Vertex desperate;

};


class AvoidVector{
    public:
        AvoidVector(){
            av.clear();
            return;
        }

        ~AvoidVector(){
            return;
        }

        void add(AvoidVectorElement newE){
            av.emplace_back(newE);
            return;
        }

        void add(Vertex cs, Vertex yield, Vertex desperate){
            AvoidVectorElement newE(cs, yield, desperate);
            this->add(newE);
            return;
        }

        bool isYield(Vertex v){
            int len = av.size();
            for(int i=0; i<len;i++){
                AvoidVectorElement tempE = av[i];
                if(tempE.getYield()==v){
                    return tempE.getCS();
                }
            }
            return -1;
        }

        bool isAvoided(Vertex cs){
            int len = av.size();
            for(int i=0; i<len;i++){
                AvoidVectorElement tempE = av[i];
                if(tempE.getCS()==cs){
                    return true;
                }
            }
            return false; 
        }

        Vertex isDesperate(Vertex v){
            int len = av.size();
            for(int i=0; i<len;i++){
                AvoidVectorElement tempE = av[i];
                if(tempE.getDesperate()==v){
                    return tempE.getCS();
                }
            }
            return -1;
        }

        void remove(Vertex yield){
            int len = av.size();
            for(int i=len-1; i>=0;i--){
                AvoidVectorElement tempE = av[i];
                if(tempE.getYield()==yield){
                    av.erase(av.begin()+i);
                }
            }
            return;
        }

        void print();

    private:
        vector<AvoidVectorElement> av;

};


#endif // DATA_STRUCTURE_
