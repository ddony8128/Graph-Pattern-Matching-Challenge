/**
 * @file performance_test.h
 *
 */

#ifndef PERFORMANCE_TEST_
#define PERFORMANCE_TEST_

#include <stdio.h>
#include <time.h>
#include <vector>
#include "common.h"
#include "graph.h"
#include "candidate_set.h"


class PerformanceTest{
    public: 
        PerformanceTest(){};  
        ~PerformanceTest(){};

        void start(){
            this->startT = time(NULL);
        }

        void end(){
            this->endT = time(NULL);
        }

        void check_result(const Graph &data, const Graph &query,
                                const CandidateSet &cs, const std::string &result_file );

        void print_result();

    private:
    
        time_t startT;
        time_t endT;
        int answer_num;
        int correct_num;
    

        bool check(const Graph &data, const Graph &query,
                                const CandidateSet &cs, const std::vector<Vertex> &answer, size_t len_query);
};



#endif // PERFORMANCE_TEST_
