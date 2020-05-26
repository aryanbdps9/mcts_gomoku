#include "utils.h"
#include "judge_and_potential.h"

#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

int judge(const vector<vector<int> > &board, int last_action, int linesize){
    int gameover;
    calc_potential(board, last_action, gameover, linesize);
    return gameover;
}



int uniform_potential(const vector<vector<int> >&board, int last_action, int &gameover, int linesize){
	calc_potential(board, last_action, gameover, linesize);
	return 1;
}


int calc_potential_disruptive(const vector<vector<int> > &board, int last_action, int &gameover, int linesize){
	int gameover_orig, gameover_disruptive;
	int potential_orig, potential_disruptive;
	potential_orig = calc_potential(board, last_action, gameover_orig, linesize);
	gameover = gameover_orig;
	
    int nr = board.size(), nc = board[0].size();
    int r  = last_action / nc, c = last_action % nc;
    int currentmove = board[r][c];
	int opp_move = 1 + (currentmove % 2);
	vector<vector<int> > board_ = board;
	board_[r][c] = opp_move;

	potential_disruptive = calc_potential(board_, last_action, gameover_disruptive, linesize);

	return potential_orig + potential_disruptive;
}


int calc_potential_disruptive2(const vector<vector<int> > &board, int last_action, int &gameover, int linesize){
	int gameover_orig, gameover_disruptive;
	int potential_orig, potential_disruptive;
	potential_orig = calc_potential2(board, last_action, gameover_orig, linesize);
	gameover = gameover_orig;
	
    int nr = board.size(), nc = board[0].size();
    int r  = last_action / nc, c = last_action % nc;
    int currentmove = board[r][c];
    assert(currentmove != 0);
	int opp_move = 1 + (currentmove % 2);
	vector<vector<int> > board_ = board;
	board_[r][c] = opp_move;

	potential_disruptive = calc_potential2(board_, last_action, gameover_disruptive, linesize);

	return potential_orig + potential_disruptive;
}


int calc_potential2(const vector<vector<int> > &board, int last_action, int &gameover, int linesize){
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

	int* fc_mat = calc_fillable(board, last_action, gameover, linesize);
	
	int count_hori = counts[2]+counts[3]+1, fill_hori = fc_mat[3]+fc_mat[2] + 1;
	int count_vert = counts[0]+counts[1]+1, fill_vert = fc_mat[0]+fc_mat[1] + 1;
	int count_maindiag = counts[4]+counts[7]+1, fill_maindiag = fc_mat[4]+fc_mat[7]+1;
	int count_othrdiag = counts[5]+counts[6]+1, fill_othrdiag = fc_mat[5]+fc_mat[7]+1;

	if (fill_hori < linesize) count_hori = 1;
	if (fill_vert < linesize) count_vert = 1;
	if (fill_maindiag < linesize) count_maindiag = 1;
	if (fill_othrdiag < linesize) count_othrdiag = 1;
	int count_max = 1;
	count_max = max(max(count_hori, count_vert), max(count_maindiag, count_othrdiag));

    if (count_max >= linesize) gameover = currentmove;
    else gameover = 0;
    return count_max;
}

int* calc_fillable(const vector<vector<int> > &board, int last_action, int &gameover, int linesize){

    // Count matrix:
	// 4 1 6
	// 3 - 2
	// 5 0 7

    int nr = board.size(), nc = board[0].size();
    int r  = last_action / nc, c = last_action % nc;
    int currentmove = board[r][c];
    assert(currentmove != 0);
	int * counts = new int[8];
    int count = 0;

	for (int row = r+1; row < r + linesize && row < nr; row++){
		// vertical top to bottom
		if (board[row][c] == currentmove || board[row][c] == 0){
			count++;
            continue;
		}
        break;
	}
	counts[0] = count;

	
	count = 0;
	for (int row = r-1; row >= r-linesize+1 && row >= 0; row--){
		// vertical bottom to top
		if (board[row][c] == currentmove || board[row][c] == 0){
			count++;
            continue;
		}
        break;
	}
	counts[1] = count;
	
	count = 0;
	for (int col = c+1; col < c + linesize && col < nc; col++){
		// horizontal left to right
		if (board[r][col] == currentmove || board[r][col] == 0){
			count++;
            continue;
		}
        break;
	}
	counts[2] = count;
	
	count = 0;
	for (int col = c-1; col >= c - linesize + 1 && col >= 0; col--){
		// horizontal right to left
		if (board[r][col] == currentmove || board[r][col] == 0){
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
		if (board[r-diff][c-diff] == currentmove || board[r-diff][c-diff] == 0){
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
		if (board[r+diff][c-diff] == currentmove || board[r+diff][c-diff] == 0){
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
		if (board[r-diff][c+diff] == currentmove || board[r-diff][c+diff] == 0){
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
		if (board[r+diff][c+diff] == currentmove || board[r+diff][c+diff] == 0){
			count++;
            continue;
		}
        break;
	}
    
	counts[7] = count;


	// 4 1 6
	// 3 - 2
	// 5 0 7
	return counts;
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