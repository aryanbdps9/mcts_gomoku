#include "utils.h"

vector<int> free_locns(vector<vector<int> > board){
    vector<int> free_actions;
    int nr = board.size(), nc = board[0].size();
    for(int i = 0; i < nr; i++){
        for(int j = 0; j < nc; j++){
			if (board[i][j] != 0){
				free_actions.push_back(nr*i+j);
			}
		}
    }
	return free_actions;
}

void print_mat(vector<vector<float> > board){
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
			cout << board[i][j] << gap;
		}
		cout << endl;
	}
}

void print_mat(vector<vector<int> > board){
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
			cout << board[i][j] << gap;
		}
		cout << endl;
	}
}

void print_mat(vector<vector<double> > board){
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
			cout << board[i][j] << gap;
		}
		cout << endl;
	}
}

void print_board(vector<vector<float> > &board){
	int N = board.size();
	// cout << "N " << N  << endl;
	string gap = "\t";
	cout << gap;
	for (int i = 0; i < N; i++){
		cout << i << gap;
	}
	cout << endl << endl;
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
int count_zeros(vector<vector<int> > board){
	int count = 0;
	for(int i = 0; i < board.size(); i++){
		for(int j = 0; j < board[i].size(); j++){
			if (!board[i][j]) count++;
		}
	}
	return count;
}

void print_board(vector<vector<int> > &board){
	int N = board.size();
	// cout << "N " << N  << endl;
	string gap = "\t";
	cout << gap;
	for (int i = 0; i < N; i++){
		cout << i << gap;
	}
	cout << endl << endl;
	string char_;
	for (int i = 0; i < N; i++){
		cout << i << gap;
		for (int j = 0; j < N; j++){
			if (board[i][j] == 1) char_ = "X";
			else if (board[i][j] == 2) char_ = "O";
			else char_ = "-";
			cout << char_ << gap;
		}
		cout << "\n" << endl;
	}
}

void print_line(int n, string c){
    for (int i = 0; i < n; i++) cout << c;
    cout << endl;
}
