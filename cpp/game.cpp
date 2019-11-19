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
	
	int r = currentpos[0];
	int c = currentpos[1];
	int currentmove = board[r][c];
	int count = 0, count_max=0;
	for (int row = max(0, r - linesize + 1); row < min(r + linesize, n); row++)
	{
		// vertical top to bottom
		if (board[row][c] == currentmove){
			count++;
		}
		else
		{
			count = 0;
		}
		if (count >= count_max){
			count_max = count;
		}
		// if (count >= linesize){
		// 	ret = currentmove;
		// 	break;
		// }
	}
	count = 0;
	for (int col = max(0, c - linesize + 1); col < min(c + linesize, n); col++)
	{
		// horizontal left to right
		if (board[r][col] == currentmove){
			count++;
		}
		else{
			count = 0;
		}
		if (count >= count_max){
			count_max = count;
		}
		// if (count >= linesize){
		// 	ret = currentmove;
		// 	break;
		// }
	}
	count = 0;
	for (int t = 0; t < 2 * linesize; t++)
	{
		// towards south east
		int roww = r - linesize + t + 1;
		int coll = c - linesize + t + 1;
		if (roww < 0 || coll < 0 || roww >= n || coll >= n)
			continue;
		if (board[roww][coll] == currentmove){
			count++;
		}
		else{
			count = 0;
		}
		if (count >= count_max){
			count_max = count;
		}
		// if (count >= linesize){
		// 	ret = currentmove;
		// 	break;
		// }
	}

	count = 0;
	for (int t = 0; t < 2 * linesize; t++)
	{
		// towards north east
		int roww = r + linesize - t - 1;
		int coll = c - linesize + t + 1;
		if (roww < 0 || coll < 0 || roww >= n || coll >= n) continue;
		if (board[roww][coll] == currentmove){
			count++;
		}
		else{
			count = 0;
		}
		if (count >= count_max){
			count_max = count;
		}
		// if (count >= linesize){
		// 	ret = currentmove;
		// 	break;
		// }
	}
	return count_max;
}



