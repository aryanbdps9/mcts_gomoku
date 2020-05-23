#ifndef Included_NameModel_Utils

#define Included_NameModel_Utils

#include <bits/stdc++.h>
using namespace std;

double get_random();

class NotImplementedException : public std::logic_error{
public:
    NotImplementedException () : std::logic_error{"Function not yet implemented."} {}
};

template <typename T>
uint32_t count_elems(vector<vector<T> > &v, T elem){
    uint32_t count = 0;
    int vs = v.size();
    for(int i = 0; i < vs; i++){
        count += std::count(v[i].begin(), v[i].end(), elem);
    }
    return count;
}

vector<int> free_locns(vector<vector<int> > board);

void print_mat(vector<vector<int> > board);
void print_mat(vector<vector<float> > board);
void print_mat(vector<vector<double> > board);

int count_zeros(vector<vector<int> > board);
void print_board(vector<vector<int> > &board);
void print_board_w_last_move(vector<vector<int> > &board, int lastpos);
void print_line(int n, string c = "#");

#endif