#include <bits/stdc++.h>
#include "game.h"
#include "tree.h"
#include "utils.h"

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

int main(int argc, char* argv[]){
	cout.precision(3);
	int board_size=atoi(argv[1]), linesize=atoi(argv[2]), num_rollouts=atoi(argv[3]), max_depth=atoi(argv[5]), timeout=atoi(argv[6]), selfplay=atoi(argv[7]), num_workers=atoi(argv[8]);
	float exploration_coeff=atof(argv[4]);
	float gamma=atof(argv[9]),alpha=atof(argv[10]);

	int N = board_size;
	float C = exploration_coeff;
	cout << N << "\t" << linesize << "\t" << num_rollouts << "\t" << C << "\t" << max_depth << "\t" << timeout << "\t" << selfplay << "\t" << num_workers << endl;

	Game game(N, linesize, gamma, alpha);
	Tree tree(&game, num_rollouts, C, max_depth, timeout, num_workers);
	int turn = 1;

	cout << "printing board\n";
	print_board(tree.root->board);
	vector<vector<int> > board;
	vector<int> pos;
	while (true){
		board = tree.root->board;
		cout << "turn of ";
		if (turn == 2) cout << "O\n";
		else cout << "X\n";
		if (turn == 2){
			if (selfplay == 0){
				pos = player_move(board);
				board = tree.player_move(pos);
				// cout <<
			}
			else{
				board = tree.play_one_move(pos);
			}
		}
		else{
			cout << "playing by computer\n";
			board = tree.play_one_move(pos);
			cout << "played by computer\n";
		}

		print_board(board);
		int judgement = tree.game->judge(board, pos);
		if (judgement){
			string plr;
			if (turn == 2) plr = "O";
			else plr = "X";
			cout << "player " << plr << " won!" << endl;
			break;
		}
		turn = (turn%2)+1;
	}
	return 0;
}
