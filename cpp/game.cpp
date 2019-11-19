#include<bits/stdc++.h>
// #include "policies.h"
#include "game.h"
#include "utils.h"

using namespace std;

Game::Game(int n, int linesize, double gamma, double alpha){
	this->n = n;
	this->linesize = linesize;
	this->gamma = gamma;
	this->alpha = alpha;
}

int Game::judge(vector<vector<int> > &board, vector<int> currentpos){
	// int rew = potential(board, currentpos);
	// if(rew >= linesize){
	// 	return board[currentpos[0]][currentpos[1]];
	// }

	int r = currentpos[0];
	int c = currentpos[1];
	int currentmove = board[r][c];
	int count_max=0;
	count_max = potential(board, currentpos);
	
	if (count_max >= linesize) return currentmove;
	return 0;
}

int Game::potential(vector<vector<int> > &board, vector<int> currentpos){
	// 4 1 6
	// 3 - 2
	// 5 0 7
	
	int r = currentpos[0];
	int c = currentpos[1];
	int currentmove = board[r][c];
	int counts[8];
	int count = 0;
	
	assert (currentmove != 0);

	for (int row = r+1; row < min(r + linesize, n) && row < n; row++){
		// vertical top to bottom
		if (board[row][c] == currentmove){
			count++;
		}
		else{
			break;
		}
	}
	counts[0] = count;

	
	count = 0;
	for (int row = r-1; row >= r-linesize+1 && row >= 0; row--){
		// vertical top to bottom
		if (board[row][c] == currentmove){
			count++;
		}
		else{
			break;
		}
	}
	counts[1] = count;
	
	count = 0;
	for (int col = c+1; col < min(c + linesize, n) && col < n; col++){
		// horizontal left to right
		if (board[r][col] == currentmove){
			count++;
		}
		else{
			break;
		}
	}
	counts[2] = count;
	
	count = 0;
	for (int col = c-1; col >= max(0, c - linesize + 1) && col >= 0; col--){
		// horizontal right to left
		if (board[r][col] == currentmove){
			count++;
		}
		else{
			break;
		}
	}
	counts[3] = count;
	
	count = 0;

	for (int diff = 1; diff < linesize && diff <= min(r,c); diff++){
		// top left
		if (board[r-diff][c-diff] == currentmove){
			count++;
		}
		else{
			break;
		}
	}
	counts[4] = count;
	// 
	count = 0;

	for (int diff = 1; diff < linesize && diff >= c && r+diff < n; diff++){
		// bottom left
		if (board[r+diff][c-diff] == currentmove){
			count++;
		}
		else{
			break;
		}
	}
	counts[5] = count;
	// 
	count = 0;

	for (int diff = 1; diff < linesize && r-diff>=0 && c+diff < n; diff++){
		// top right
		if (board[r-diff][c+diff] == currentmove){
			count++;
		}
		else{
			break;
		}
	}
	counts[6] = count;
	// 
	count = 0;

	for (int diff = 1; diff < linesize && r+diff < n && c+diff < n; diff++){
		// bottom right
		if (board[r+diff][c+diff] == currentmove){
			count++;
		}
		else{
			break;
		}
	}
	counts[7] = count;
	// 4 1 6
	// 3 - 2
	// 5 0 7
	int count_max = 0;
	count_max = max(counts[0]+counts[1]+1, max(counts[2]+counts[3] + 1, counts[4]+counts[7]+1));
	count_max = max(counts[5]+counts[6]+1, count_max);
	
	return count_max;
}



