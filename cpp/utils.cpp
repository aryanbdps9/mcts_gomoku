#include "utils.h"


void print_board(vector<vector<int> > &board){
	int N = board.size();
	// cout << "N " << N  << endl;
	string gap = "\t";
	cout << gap;
	for (int i = 0; i < N; i++){
		cout << i << gap;
	}
	cout << endl;
	string char_;
	for (int i = 0; i < N; i++){
		cout << i << gap;
		for (int j = 0; j < N; j++){
			if (board[i][j] == 1) char_ = "X";
			else if (board[i][j] == 2) char_ = "O";
			else char_ = "-";
			cout << char_ << gap;
		}
		cout << endl;
	}
}

void print_line(int n, string c){
    for (int i = 0; i < n; i++) cout << c;
    cout << endl;
}