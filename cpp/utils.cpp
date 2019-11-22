#include "utils.h"

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
		cout << "\n" << endl;
	}
}

void print_line(int n, string c){
    for (int i = 0; i < n; i++) cout << c;
    cout << endl;
}

vector<int> get_flags(unsigned int input, int k){
	int cnt = 0;
	vector<int> ret;
	for(int i = 0; i < sizeof(int)*8 && cnt < k; i++) {
		if (input & 0x01){
			cnt++;
			ret.push_back(i);
		}
		input >>= 1;
	}
	return ret;
}

Mode modeint2enum(int mode){
	if (mode == 1)
		return Vanilla;
	else if (mode == 2)
		return RewShape;
	else if (mode == 3)
		return Prior;
	else {
		cerr << "mode is invalid\n";
		exit(-1);
	}
}