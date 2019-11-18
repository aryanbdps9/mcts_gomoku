
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
	policy *policies[2] = {new policy_random(), new policy_random()};
    Game(int n, int linesize);

	int judge(vector<vector<int> > &board, vector<int> currentpos);
};


#endif
