#ifndef Included_NameModel_JAP

#define Included_NameModel_JAP


#include <bits/stdc++.h>
using namespace std;

int judge(const vector<vector<int> > &board, int last_action, int linesize);

int uniform_potential(const vector<vector<int> >&board, int last_action, int &gameover, int linesize);

int calc_potential_disruptive(const vector<vector<int> > &board, int last_action, int &gameover, int linesize);


int calc_potential_disruptive2(const vector<vector<int> > &board, int last_action, int &gameover, int linesize);

int calc_potential2(const vector<vector<int> > &board, int last_action, int &gameover, int linesize);

int* calc_fillable(const vector<vector<int> > &board, int last_action, int &gameover, int linesize);

int calc_potential(const vector<vector<int> > &board, int last_action, int &gameover, int linesize);

#endif