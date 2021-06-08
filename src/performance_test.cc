/**
 * @file performance_test.cc
 *
 */

#include <stdio.h>
#include "performance_test.h"
#include "backtrack.h"
#include "common.h"
#include <vector>


void PerformanceTest::check_result(const Graph &data, const Graph &query,
                                const CandidateSet &cs, const std::string &result_file){
    int answer_num = 0;
    int correct_num = 0;
    
    std::ifstream file(result_file);

    if (!file.is_open()){
        std::cout << "Result file "<< result_file << " not found.\n";
        exit(EXIT_FAILURE);
    }

    char type;
    size_t len_query;

    

    file >> type >> len_query;
    
    std::vector<Vertex> temp_answer;
    temp_answer.resize(len_query);

    while(file >> type){
        if (type == 'a'){
            temp_answer.clear();

            for (size_t i=0; i<len_query;i++){
                Vertex v;
                file >> v;
                temp_answer.emplace_back(v);
            }
            
            answer_num++;
            if (this->check(data, query, cs, temp_answer, len_query)){
                correct_num++;
            } else{
                std::cout<<"wrong "<<answer_num;    
            }

        }
    }

    file.close();
    
    this->answer_num = answer_num;
    this->correct_num = correct_num;


    return;
}

void PerformanceTest::print_result(){
    std::cout<<"\n";
    std::cout<<"correctness rate: "<<this->correct_num<<"/"<<this->answer_num<<"\n";
    
    int time = (int)(double)(this->endT - this->startT);
    int min = time/60;
    int sec = time%60;
    
    std::cout<<"time: "<<min<<"minutes, "<<sec<<"seconds."<<"\n";

    return;
}

bool PerformanceTest::check(const Graph &data, const Graph &query,
                                const CandidateSet &cs, const std::vector<Vertex> &answer, size_t len_query){
    for(size_t k = 0;k<len_query;k++){
        bool exist = false;
        
        for(size_t i=0;i<cs.GetCandidateSize(k);i++){
            if (cs.GetCandidate(k, i)==answer[k]){
                exist = true;
            }
        }

        if (!exist){
            return false;
        }

        for (size_t i=query.GetNeighborStartOffset(k);i<query.GetNeighborEndOffset(k);i++){
            Vertex temp_vertex = query.GetNeighbor(i);
            Vertex temp_candidate = answer[temp_vertex];
            if (!(data.IsNeighbor(answer[k],temp_candidate))){
                return false;
            }
        }

    }
    return true;
}
