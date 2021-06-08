/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include "data_structure.h"
#include "stdlib.h"
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>


#define MAXINT 100000000

using namespace std;

SearchStack *search_stack;
size_t len_query;
ActivatedBitArray *activated_bit_arr;
BoundaryCountArray *boundary_count_arr;
IncomingNumberArray *incoming_number_arr;
size_t activated_count;
SelectedArray *selected_arr;
AvoidVector *avoid_vector;
bool finish_flag;
vector<vector<Vertex>> DAG;
vector<vector<Vertex>> RDAG;


Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}  

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  // Prepare and initialize data structures                   
  search_stack = new SearchStack();
  len_query = query.GetNumVertices();
  activated_bit_arr = new ActivatedBitArray(len_query);
  boundary_count_arr = new BoundaryCountArray(len_query);
  incoming_number_arr = new IncomingNumberArray(len_query);
  activated_count = 0;
  selected_arr = new SelectedArray(len_query, data);
  avoid_vector = new AvoidVector();
  finish_flag = false; 
                         
  Vertex root;
  // Find min_root that satisfies the condition
  root = RootForDAG(query, cs);

  // Build DAG and RDAG
  DAG.resize(query.GetNumVertices());
  RDAG.resize(query.GetNumVertices());
  BuildDAG(query, cs, root, DAG);
  BuildReverseDAG(DAG, RDAG);

  bool con;
  
  // Main loop
  while (1) {
    con = false;

    // If activated_count is equal to len_query, print out the answer and goto bracktrack_3()
    if (activated_count == len_query) {
      selected_arr->print_result();
      
      finish_flag = backtrack_3();
      // If finish_flag is 1, end of the search
      if (finish_flag) {
        delete search_stack;
        return;
      } else {
        continue;
      }
    }
    // Prepare temp_smallest_extendable, temp_smallest_cs, temp_smallest_cs_num
    Vertex temp_smallest_extendable;
    vector<Vertex>* temp_smallest_cs;
    
    int temp_smallest_cs_num = MAXINT;
    
    vector<Vertex> extendable_vertex;
    
    
    for (int k = 0; k < len_query; k++) {
      // Find the extendable vertex
      if( ( !activated_bit_arr->isActivated(k) ) && boundary_count_arr->getBoundaryCount(k) == incoming_number_arr->getIncomingNumber(k)) {

        vector<Vertex> illuminated_cs;
        vector<Vertex> *temp_cs;
    
        temp_cs = new vector<Vertex>();

        // Get candidate set of vertex k to temp_cs
        for (int i = 0; i < cs.GetCandidateSize(k); i++) {
          temp_cs->emplace_back(cs.GetCandidate(k, i));
        }
        // If the the candidate set node is in the selected_array or avoid_vector, set illuminated_cs and erase from temp_cs
        for (int j = temp_cs->size()-1; j>= 0; j--) {
              if (selected_arr->isSelected( (*temp_cs)[j]) || avoid_vector->isAvoided((*temp_cs)[j])) {
                illuminated_cs.emplace_back((*temp_cs)[j]);
                temp_cs->erase(temp_cs->begin() + j);
                
              }
        }

     
        // If no more temp_cs left, perform backtrack_1()
        if (temp_cs->size() == 0) {
          finish_flag = backtrack_1(data, query, cs, illuminated_cs, k);
          // If finish_flag is 1, end of the search
          if (finish_flag) {
            delete search_stack;
            return;
          } else {
            k = len_query;
            con = true;
            continue;
          }
        }



        // Find parent of k using RDAG and only leave the cs that have all edges with the selected parents
        for (int i = 0; i<RDAG[k].size() ; i++){
          Vertex parent = RDAG[k][i];
          Vertex parent_cs = selected_arr->getSelectedOf(parent);
          for (int j = temp_cs->size()-1; j>=0; j--){
            if (!data.IsNeighbor(parent_cs, (*temp_cs)[j])){
              temp_cs->erase(temp_cs->begin() + j);
            }
          }
        }


    
        // If no more temp_cs left, perform backtrack_1()
        if (temp_cs->size() == 0) {
          finish_flag = backtrack_2(data, query, cs, illuminated_cs, k);
          // If finish_flag is 1, end of the search
          if (finish_flag) {
            delete search_stack;
            return;
          } else {
            k = len_query;
            con = true;
            continue;
          }
        }
    
        // Set the information of temp_smallest_extendable, temp_smallest_cs_num, temp_smallest_cs
        if (temp_cs->size() < temp_smallest_cs_num) {
          temp_smallest_extendable = k;
          temp_smallest_cs_num = temp_cs->size();
          temp_smallest_cs = new vector<Vertex>(*temp_cs);
          
        } else{
          delete temp_cs;
    
        }
        
      }
      
    } 

    if (con){
      continue;
    }
    // Prepare next_cs and selected_cs
    vector<Vertex> next_cs;
    Vertex selected_cs;
    
    

    int first_cs;
    size_t smallest_edge;
    int end = temp_smallest_cs->size();
    

    // Fill in the selected_cs and next_cs according to the degree size
    for (int i=end;i>0;i--){
      first_cs = 0;
      smallest_edge = data.GetDegree((*temp_smallest_cs)[0]);
      
                

      for (int k=1;k<i;k++){
      size_t temp = data.GetDegree( (*temp_smallest_cs)[k]);
        if(temp<smallest_edge){
          smallest_edge = temp;
          first_cs = k;
        }
      }

      

      if (i==end){
        selected_cs = (*temp_smallest_cs)[first_cs];
      } else {
        next_cs.emplace_back( (*temp_smallest_cs)[first_cs]);
      }

      temp_smallest_cs->erase((temp_smallest_cs->begin())+first_cs);

    
    }

    // push the SearchStackElement to the search_stack and activate activated_bit_arr, boundary_count_arr, selected_arr, activated_count
    search_stack->push(SearchStackElement(temp_smallest_extendable, selected_cs, next_cs));
    activated_bit_arr->activate(temp_smallest_extendable);
    boundary_count_arr->activate(DAG, temp_smallest_extendable);
    selected_arr->select(temp_smallest_extendable, selected_cs);
    activated_count++;


    

  }


}



// Find the root for DAG which has minimun cs.GetCandidateSize()/query.GetDegree(0)
Vertex Backtrack::RootForDAG(const Graph &query, const CandidateSet &cs) {
  Vertex rroot = 0;
  float compare = (float)(cs.GetCandidateSize(0))/(query.GetDegree(0));
  for (int i = 1; i<query.GetNumVertices(); i++) {
    rroot = ((float)(cs.GetCandidateSize(i))/(query.GetDegree(i)) < compare)? i:rroot;
    compare = ((float)(cs.GetCandidateSize(i))/(query.GetDegree(i)) < compare)? (float)(cs.GetCandidateSize(i))/(query.GetDegree(i)):compare;

  }
  return rroot;
}


// BuildDAG in the BFS order
void Backtrack::BuildDAG(const Graph &query, const CandidateSet &cs, Vertex root, vector<vector<Vertex>> &DAG) {
  bool **visit;
  visit = new bool*[query.GetNumVertices()];

  for (size_t i = 0; i < query.GetNumVertices(); i++) {
    visit[i] = new bool[query.GetNumVertices()];
  }

  for (size_t i = 0; i < query.GetNumVertices(); i++) {
    for (size_t j = 0; j< query.GetNumVertices(); j++){
      visit[i][j] = false;
    }
  }

  queue<int> q;
  q.push(root);
  visit[root][root] = true;

  while(!q.empty()) {
    int x = q.front();
    q.pop();

    for ( int i = query.GetNeighborStartOffset(x); i < query.GetNeighborEndOffset(x); i++ ) {
      int y = query.GetNeighbor(i);
      if (!visit[x][y]){
        visit[x][y] = true;
        visit[y][x] = true;
        // Also establish the incoming_number_arr
        incoming_number_arr->count(y);
        DAG[x].emplace_back(y);
      }
      if (!visit[y][y]) {
        q.push(y);
        visit[y][y] = true;
      }
    }

  }
}

// Build RDAG simply using existing DAG
void Backtrack::BuildReverseDAG(vector<vector<Vertex>> &DAG, vector<vector<Vertex>> &RDAG){
  for (int i = 0; i<DAG.size();i++){
    for (int j = 0; j<DAG[i].size();j++){
      Vertex child = DAG[i][j];
      RDAG[child].emplace_back(i);
    }
  }
}

// Backtrack #1
bool Backtrack::backtrack_1(const Graph &data, const Graph &query,
                                const CandidateSet &cs, vector<Vertex> &illuminated_cs, Vertex k) {
  
  SearchStackElement temp;
  bool is_in_illuminated_cs = false;

  while (!is_in_illuminated_cs) {
    // If search_stack isEmpty, return true which means the end of search
    if (search_stack->isEmpty()) {
      return true;
    }
    // Pop() search_stack
    temp = search_stack->pop();  
    
    // Check if the selected_cs of popped search_stack is in illuminated vector
    Vertex temp_selected_cs = temp.getSelectedCS();
    for (int i = 0; i < illuminated_cs.size(); i++) {
      if (temp_selected_cs == illuminated_cs[i]) {
        is_in_illuminated_cs = true;
        i = illuminated_cs.size();
      }
    }
    // If not illuminated, inactivate the query node and go to the start of while loop
    if (!is_in_illuminated_cs){
      inactivate(temp.getQueryNode());
    }

  }

  
  // Add the avoid_vector_element to the avoid_vector
  AvoidVectorElement avoid_vector_element(temp.getSelectedCS(), temp.getQueryNode(), k);
  avoid_vector->add(avoid_vector_element);

  while (true){
    // Pop() the NextCS until empty and set it to selected_cs if not in avoid vector
    while (!temp.isEmptyNextCS()) {
        Vertex temp_next = temp.popNextCS();
        if (!avoid_vector->isAvoided(temp_next)) {
          temp.setSelectedCS(temp_next);
   
          search_stack->push(temp);
          
          selected_arr->select(temp.getQueryNode(), temp_next);

          return false;
        } 
      }
    // Inactivate the query node
    inactivate(temp.getQueryNode());
    
    if (search_stack->isEmpty()) {
      return true;
    } 
    // Pop() the search_stack if not empty and go to the start of while loop
    temp = search_stack->pop();
    
  }
  
  return false;

}

// Backtrack #2
bool Backtrack::backtrack_2(const Graph &data, const Graph &query,
                                const CandidateSet &cs, vector<Vertex> &illuminated_cs, Vertex k) {

  SearchStackElement temp;
  bool is_in_illuminated;

  bool con = true;
  while (con) {
    // If search_stack isEmpty, return true which means the end of search
    if (search_stack->isEmpty()) {
      return true;
    }
    // Pop() search_stack
    temp = search_stack->pop();
    is_in_illuminated = false;
    // Check if the selected_cs of popped search_stack is in illuminated vector
    Vertex temp_selected_cs = temp.getSelectedCS();
    for (int i = 0; i < illuminated_cs.size(); i++) {
      if (temp_selected_cs == illuminated_cs[i]) {
        is_in_illuminated = true;
        i = illuminated_cs.size();
      }
    }

    // If the selected_cs is in illumated vector of the query node is the parent of k, go to the next while loop
    if (is_in_illuminated || query.IsNeighbor(temp.getQueryNode(), k)){
      con = false;
    } else{
      // Else inactivate the query node and to the start of while loop
      inactivate(temp.getQueryNode());
    } 
      
  } 


  while(true){
      // Pop() the NextCS until empty and set it to selected_cs if not in avoid vector
     while (!temp.isEmptyNextCS()) {
      Vertex temp_next = temp.popNextCS();
      if (!avoid_vector->isAvoided(temp_next)) {
        temp.setSelectedCS(temp_next);

        search_stack->push(temp);

        
        
        selected_arr->select(temp.getQueryNode(), temp_next);
        return false;
      } 
    }
    // Inactivate the query node
    inactivate(temp.getQueryNode());

    if (search_stack->isEmpty()) {
      return true;
    }
    // Pop() the search_stack if not empty and go to the start of while loop
    temp = search_stack->pop();
  }
  
  return false;

}

// Backtrack #3
bool Backtrack::backtrack_3(){

  SearchStackElement temp;


  while (true) {
    // If search_stack isEmpty, return true which means the end of search
    if (search_stack->isEmpty()) {
      return true;
    }
    // Pop() search_stack
    temp = search_stack->pop();
    // Pop() the NextCS until empty and set it to selected_cs if not in avoid vector
    while (!temp.isEmptyNextCS()) {
      Vertex temp_next = temp.popNextCS();
      if (!avoid_vector->isAvoided(temp_next)) {
        temp.setSelectedCS(temp_next);
        search_stack->push(temp);
        selected_arr->select(temp.getQueryNode(), temp_next);
        return false;
      } 
    }
    // Inactivate the query node
    inactivate(temp.getQueryNode());

  }  
  return false;
  
}

// Inactivate the data structures of the denied vertex
void Backtrack::inactivate(Vertex denied){
  avoid_vector->remove(denied);
  activated_bit_arr->inactivate(denied);
  boundary_count_arr->inactivate(DAG, denied);
  selected_arr->unselect(denied);
  activated_count--;

}
