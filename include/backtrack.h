/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include <stack>

using namespace std;

class Backtrack {
    public:
        Backtrack();
        ~Backtrack();

        void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs);
    private:


        Vertex RootForDAG(const Graph &query, const CandidateSet &cs);

        void BuildDAG(const Graph &query, const CandidateSet &cs, Vertex root, vector<vector<Vertex>> &DAG);

        void BuildReverseDAG(vector<vector<Vertex>> &DAG, vector<vector<Vertex>> &RDAG);

        bool backtrack_1(const Graph &data, const Graph &query,
                                const CandidateSet &cs, vector<Vertex> &illuminated_cs, Vertex k);

        bool backtrack_2(const Graph &data, const Graph &query,
                                const CandidateSet &cs, vector<Vertex> &illuminated_cs, Vertex k);

        bool backtrack_3();

        void inactivate(Vertex denied);

};

#endif  // BACKTRACK_H_
