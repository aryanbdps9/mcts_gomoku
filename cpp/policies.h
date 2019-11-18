#ifndef Included_NameModel_Policies

#define Included_NameModel_Policies
#include<bits/stdc++.h>

using namespace std;


// Existing code goes here


class policy{
	public:
	vector<int> get_action(vector<vector<int> > &board, int current_player);
};

class policy_random : public policy{
	vector<int> get_action(vector<vector<int> > &board, int current_player);
};

#endif

