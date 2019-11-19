#include<bits/stdc++.h>
// #include "policies.h"
#include "game.h"
#include "utils.h"

using namespace std;

Game::Game(int n, int linesize){
	this->n = n;
	this->linesize = linesize;
}

	int Game::judge(vector<vector<int> > &board, vector<int> currentpos){
		int r = currentpos[0];
		int c = currentpos[1];
		int currentmove = board[r][c];
		int count = 0;
		int ret = 0;
		for(int row = max(0, r-linesize+1); row < min(r+linesize, n); row++){
			// vertical top to bottom
			if(count >= 5){
				ret = currentmove;
			}
			if(board[row][c] == currentmove){
				count++;
			}
			else{
				count = 0;
			}
		}
		count = 0;
		for(int col = max(0, c-linesize+1); col < min(c+linesize, n); col++){
			// horizontal left to right
			if(count >= 5){
				ret = currentmove;
			}
			if(board[r][col] == currentmove){
				count++;
			}
			else{
				count = 0;
			}
		}
		count = 0;
		for (int t = 0; t < 2*linesize; t++){
			// towards south east
			int roww = r - linesize + t + 1;
			int coll = c - linesize + t + 1;
			if (roww < 0 || coll < 0 || roww >= n || coll >= n) continue;
			if(count >= 5){
				ret = currentmove;
			}
			if(board[roww][coll] == currentmove){
				count++;
			}
			else{
				count = 0;
			}
		}

		count = 0;
		for (int t = 0; t < 2*linesize; t++){
			// towards north east
			int roww = r + linesize - t - 1;
			int coll = c - linesize + t + 1;
			if (roww < 0 || coll < 0 || roww >= n || coll >= n) continue;
			if(count >= 5){
				ret = currentmove;
			}
			if(board[roww][coll] == currentmove){
				count++;
			}
			else{
				count = 0;
			}
		}
		return ret;
	}

