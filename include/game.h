
#ifndef Included_NameModel_Game

#define Included_NameModel_Game
#include<bits/stdc++.h>
#include "policies.h"

using namespace std;
// Existing code goes here

class Game{
	public:
	int n;
	int linesize;
	int verbose = 0;
	policy *policies[2] = {new policy_random(), new policy_random()};
    Game(int n, int linesize, int verbose);

	int judge(vector<vector<int> > &board, vector<int> currentpos);
	int potential(vector<vector<int> > &board, vector<int> currentpos);
};

#endif
