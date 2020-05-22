#ifndef Included_NameModel_JAP

#define Included_NameModel_JAP


#include <bits/stdc++.h>
using namespace std;

int judge(const vector<vector<int> > &board, int last_action, int linesize);


int calc_potential(const vector<vector<int> > &board, int last_action, int &gameover, int linesize);

#endif