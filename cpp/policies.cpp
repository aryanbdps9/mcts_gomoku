#include<bits/stdc++.h>
#include "policies.h"

using namespace std;

vector<int> policy::get_action(vector<vector<int> > &board, int current_player){
	vector<int> ret;
	return ret;
}

vector<int> policy_random::get_action(vector<vector<int> > &board, int current_player){
	int N = board.size();
	vector<vector<int> > free_locns;
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			if(board[i][j] == 0){
				vector<int> pos = {i, j};
				free_locns.push_back(pos);
			}
		}
	}
	if(free_locns.size() == 0){
		vector<int> pos = {-1, -1};
		return pos;
	}

	int choice = rand() % free_locns.size();
	return free_locns[choice];
}


