#include<bits/stdc++.h>
#include "utils.h"
#include "judge_and_potential.h"

using namespace std;

int judge(const vector<vector<int> > &board, int last_action, int linesize){
    int gameover;
    calc_potential(board, last_action, gameover, linesize);
    return gameover;
}

int calc_potential(const vector<vector<int> > &board, int last_action, int &gameover, int linesize){

    // Count matrix:
	// 4 1 6
	// 3 - 2
	// 5 0 7

    int nr = board.size(), nc = board[0].size();
    int r  = last_action / nc, c = last_action % nc;
    int currentmove = board[r][c];
    assert(currentmove != 0);
    int counts[8];
    int count = 0;

	for (int row = r+1; row < r + linesize && row < nr; row++){
		// vertical top to bottom
		if (board[row][c] == currentmove){
			count++;
            continue;
		}
        break;
	}
	counts[0] = count;

	
	count = 0;
	for (int row = r-1; row >= r-linesize+1 && row >= 0; row--){
		// vertical bottom to top
		if (board[row][c] == currentmove){
			count++;
            continue;
		}
        break;
	}
	counts[1] = count;
	
	count = 0;
	for (int col = c+1; col < c + linesize && col < nc; col++){
		// horizontal left to right
		if (board[r][col] == currentmove){
			count++;
            continue;
		}
        break;
	}
	counts[2] = count;
	
	count = 0;
	for (int col = c-1; col >= c - linesize + 1 && col >= 0; col--){
		// horizontal right to left
		if (board[r][col] == currentmove){
			count++;
            continue;
		}
        break;
	}
	counts[3] = count;
	
	count = 0;
    int mrc = min(r,c);

	for (int diff = 1; diff < linesize && diff <= mrc; diff++){
		// top left
		if (board[r-diff][c-diff] == currentmove){
			count++;
            continue;
		}
        break;
	}
	counts[4] = count;
	// 
	count = 0;

	for (int diff = 1; diff < linesize && c >= diff && r+diff < nr; diff++){
		// bottom left
		if (board[r+diff][c-diff] == currentmove){
			count++;
            continue;
		}
        break;
	}
	counts[5] = count;
	// 
	count = 0;

	for (int diff = 1; diff < linesize && r-diff>=0 && c+diff < nc; diff++){
		// top right
		if (board[r-diff][c+diff] == currentmove){
			count++;
            continue;
		}
        break;
	}
	counts[6] = count;
	// 
	count = 0;

	for (int diff = 1; diff < linesize && r+diff < nr && c+diff < nc; diff++){
		// bottom right
		if (board[r+diff][c+diff] == currentmove){
			count++;
            continue;
		}
        break;
	}
    
	counts[7] = count;


	// 4 1 6
	// 3 - 2
	// 5 0 7
	int count_max = 0;
	count_max = max(counts[0]+counts[1]+1, max(counts[2]+counts[3] + 1, counts[4]+counts[7]+1));
	count_max = max(counts[5]+counts[6]+1, count_max);

    if (count_max >= linesize) gameover = currentmove;
    else gameover = 0;
    return count_max;
}