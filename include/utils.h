#ifndef Included_NameModel_Utils

#define Included_NameModel_Utils

#include <bits/stdc++.h>
using namespace std;

class NotImplementedException : public std::logic_error{
public:
    NotImplementedException () : std::logic_error{"Function not yet implemented."} {}
};

vector<int> free_locns(vector<vector<int> > board);

void print_mat(vector<vector<int> > board);
void print_mat(vector<vector<float> > board);
void print_mat(vector<vector<double> > board);

int count_zeros(vector<vector<int> > board);
void print_board(vector<vector<int> > &board);
void print_line(int n, string c = "#");
vector<int> get_flags(unsigned int input, int k=2);

#endif