#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <string>

#include "utils.h"


using namespace std;

double get_random() { return (double)rand() / (double)RAND_MAX; }

string toLower(string s){
	string s_(s);
	std::transform(s_.begin(), s_.end(), s_.begin(),
    [](unsigned char c){ return std::tolower(c); }); // copied from https://stackoverflow.com/a/313990/7736191
	return s_;
}

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

void print_board_w_last_move(vector<vector<int> > &board, int lastpos){
	int nr = board.size(), nc = board[0].size();
	// cout << "N " << N  << endl;
	string gap = "    ";
	string small_gap = "   ";
	cout << gap;
	for (int i = 0; i < nc; i++){
		cout << i << gap;
	}
	cout << endl << endl;
	string char_;
	int curpos = 0;
	for (int i = 0; i < nr; i++){
		if (curpos == lastpos){
			cout << i << small_gap;
		}
		else{
			cout << i << gap;
		}
		for (int j = 0; j < nc; j++, curpos++){
			if (board[i][j] == 1) char_ = "X";
			else if (board[i][j] == 2) char_ = "O";
			else char_ = "-";

			if (curpos == lastpos - 1)
				cout << char_ << small_gap;
			else if (curpos == lastpos)
				cout << "[" << char_ << "]" << small_gap;
			else
				cout << char_ << gap;
		}
		cout << "\n" << endl;
	}
}

void print_line(int n, string c){
    for (int i = 0; i < n; i++) cout << c;
    cout << endl;
}
