#include <bits/stdc++.h>
#include "game.h"
#include "tree.h"

using namespace std;

vector<int> player_move(vector<vector<int> > &board){
	int N = board.size();
	vector<int> pos(2);
	while (true){
		cout << "Enter your new mark position(pair of space separated integers):\t";
		cin >> pos[0] >> pos[1];
		if (pos[0] < 0 || pos[0] >= N || pos[1] < 0 || pos[1] >= N){
			cerr << "ERROR: Enter a pair of integers within board range\n";
			continue;
		}
		if (board[pos[0]][pos[1]] != 0){
			cerr << "ERROR: Enter a position that is empty\n";
			continue;
		}
		return pos;
	}
}

void print_board(vector<vector<int> > &board){
	int N = board.size();
	cout << "N " << N  << endl;
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

int main(int argc, char* argv[]){
	int board_size=atoi(argv[1]), linesize=atoi(argv[2]), num_rollouts=atoi(argv[3]), max_depth=atoi(argv[5]), timeout=atoi(argv[6]), selfplay=atoi(argv[7]), num_workers=atoi(argv[8]);
	float exploration_coeff=atof(argv[4]);

	int N = board_size;
	float C = exploration_coeff;
	cout << N << "\t" << linesize << "\t" << num_rollouts << "\t" << C << "\t" << max_depth << "\t" << timeout << "\t" << selfplay << "\t" << num_workers << endl;

	Game game(N, linesize);
	Tree tree(&game, num_rollouts, C, max_depth, timeout, num_workers);
	int turn = 1;

	cout << "printing board\n";
	print_board(tree.root->board);
	vector<vector<int> > board;
	while (true){
		board = tree.root->board;
		cout << "turn of ";
		if (turn == 2) cout << "O\n";
		else cout << "X\n";
		if (turn == 2){
			if (selfplay == 0){
				vector<int> pos = player_move(board);
				board = tree.player_move(pos);
				// cout <<
			}
			else{
				board = tree.play_one_move();
			}
		}
		else{
			cout << "playing by computer\n";
			board = tree.play_one_move();
			cout << "played by computer\n";
		}

		print_board(board);
		if (tree.root->gameover != 0){
			string plr;
			if (turn == 2) plr = "O";
			else plr = "X";
			cout << "player " << plr << "won!" << endl;
			break;
		}
		turn = (turn%2)+1;
	}
	return 0;
}
