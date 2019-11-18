#include<bits/stdc++.h>
// #include "policies.h"
#include "game.h"

using namespace std;

Game::Game(int n, int linesize){
	this->n = n;
	this->linesize = linesize;
}

	int Game::judge(vector<vector<int> > &board, vector<int> currentpos){
		int r = currentpos[0];
		int c = currentpos[1]; // it was 0. Aryan changed it to 1
		int currentmove = board[r][c];
		int count = 0;
		for(int row = max(0, r-linesize+1); row < min(r+linesize, n); row++){
			if(count >= 5){
				return currentmove;
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
			if(count >= 5){
				return currentmove;
			}
			if(board[r][col] == currentmove){
				count++;
			}
			else{
				count = 0;
			}
		}
		
		count = 0;
		for(int t = max(max(-linesize+1,-r),-c); t < min(min(linesize,n-r), n-c); t++){
			if(count >= 5){
				return currentmove;
			}
			if(board[r+t][c+t] == currentmove){
				count++;
			}
			else{
				count = 0;
			}
		}
		
		count = 0;
		for(int t = max(max(-linesize+1,r-n),-c); t < min(min(linesize,r),n-c); t++){
			if(count >= 5){
				return currentmove;
			}
			if(board[r-t][c+t] == currentmove){
				count++;
			}
			else{
				count = 0;
			}
		}
		return 0;
	}

